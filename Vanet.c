#include <ip.h>
#include <aodv/aodv.h>
#include <aodv/aodv_packet.h>
#include <random.h>
#include <cmu-trace.h>
//#include <energy-model.h>

#define max(a,b) ( (a) > (b) ? (a) : (b) )
#define CURRENT_TIME Scheduler::instance().clock()

//#define DEBUG
//#define ERROR
#ifdef DEBUG
static int inroute_request = 0;
#endif

/*
TCL Hooks
*/
int hdr_aodv::offset_;

static class AODVHeaderClass : public PacketHeaderClass {
public:
    AODVHeaderClass() : PacketHeaderClass("PacketHeader/AODV", sizeof(hdr_all_aodv)) {
        bind_offset(&hdr_aodv::offset_);
    }
} class_rtProtoAODV_hdr;

static class AODVclass : public TclClass {
public:
    AODVclass() : TclClass("Agent/AODV") {}
    TclObject* create(int argc, const char*const* argv) {
        assert(argc == 5);
        //return (new AODV((nsaddr_t) atoi(argv[4])));
        return (new AODV((nsaddr_t) Address::instance().str2addr(argv[4])));
    }
} class_rtProtoAODV;

int AODV::command(int argc, const char*const* argv) {
    if(argc == 2) {
        Tcl& tcl = Tcl::instance();

        if(strncasecmp(argv[1], "id", 2) == 0) {
            tcl.resultf("%d", index);
            return TCL_OK;
        }
        if(strncasecmp(argv[1], "blackhole", 9) == 0) {
            malicious=1000;
            return TCL_OK;
        }
        if(strncasecmp(argv[1], "start", 2) == 0) {
            btimer.handle((Event*) 0);

            #ifndef AODV_LINK_LAYER_DETECTION
            htimer.handle((Event*) 0);
            ntimer.handle((Event*) 0);
            #endif // LINK LAYER DETECTION

            rtimer.handle((Event*) 0);
            return TCL_OK;
        }
    }
    else if(argc == 3) {
        if(strcmp(argv[1], "index") == 0) {
            index = atoi(argv[2]);
            return TCL_OK;
        }

        else if(strcmp(argv[1], "log-target") == 0 || strcmp(argv[1], "tracetarget") == 0) {
            logtarget = (Trace*) TclObject::lookup(argv[2]);
            if(logtarget == 0)
                return TCL_ERROR;
            return TCL_OK;
        }
        else if(strcmp(argv[1], "drop-target") == 0) {
            int stat = rqueue.command(argc,argv);
            if (stat != TCL_OK) return stat;
            return Agent::command(argc, argv);
        }

        else if(strcmp(argv[1], "if-queue") == 0) {
            ifqueue = (PriQueue*) TclObject::lookup(argv[2]);

            if(ifqueue == 0)
                return TCL_ERROR;
            return TCL_OK;
        }
        else if (strcmp(argv[1], "port-dmux") == 0) {
            dmux_ = (PortClassifier *)TclObject::lookup(argv[2]);
            if (dmux_ == 0) {
                fprintf (stderr, "%s: %s lookup of %s failed\n", __FILE__, argv[1], argv[2]);
                return TCL_ERROR;
            }
            return TCL_OK;
        }
    }
    return Agent::command(argc, argv);
}

/*
Constructor
*/
AODV::AODV(nsaddr_t id) : Agent(PT_AODV), btimer(this), htimer(this), ntimer(this), rtimer(this), lrtimer(this), rqueue() {
    index = id;
    seqno = 2;
    bid = 1;
    LIST_INIT(&nbhead);
    LIST_INIT(&bihead);

    malicious=999;
    logtarget = 0;

    ifqueue = 0;
}

/*
Timers
*/
void BroadcastTimer::handle(Event*) {
    agent->id_purge();
    Scheduler::instance().schedule(this, &intr, BCAST_ID_SAVE);
}

void HelloTimer::handle(Event*) {
    agent->sendHello();
    double interval = MinHelloInterval + ((MaxHelloInterval - MinHelloInterval) * Random::uniform());
    assert(interval >= 0);

    Scheduler::instance().schedule(this, &intr, interval);
}

void NeighborTimer::handle(Event*) {
    agent->nb_purge();
    Scheduler::instance().schedule(this, &intr, HELLO_INTERVAL);
}

void RouteCacheTimer::handle(Event*) {
    agent->rt_purge();
    #define FREQUENCY 0.5 // sec
    Scheduler::instance().schedule(this, &intr, FREQUENCY);
}

void LocalRepairTimer::handle(Event* p) {
    aodv_rt_entry *rt;
    struct hdr_ip *ih = HDR_IP((Packet *)p);

    /* you get here after the timeout in a local repair attempt */
    /* fprintf(stderr, "%s\n", __FUNCTION__); */
    rt = agent->rtable.rt_lookup(ih->daddr());
    if (rt && rt->rt_flags != RTF_UP) {
        // route is yet to be repaired
        // I will be conservative and bring down the route
        // and send route errors upstream.
        /* The following assert fails, not sure why */
        /* assert (rt->rt_flags == RTF_IN_REPAIR); */

        //rt->rt_seqno++;
        agent->rt_down(rt);
        // send RERR
        #ifdef DEBUG
        fprintf(stderr,"Dst - %d, failed local repair\n", rt->rt_dst);
        #endif
    }
    Packet::free((Packet *)p);
}
