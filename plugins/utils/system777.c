//
//  system777.c
//  crypto777
//
//  Created by James on 4/9/15.
//  Copyright (c) 2015 jl777. All rights reserved.
//

#ifdef DEFINES_ONLY
#ifndef crypto777_system777_h
#define crypto777_system777_h

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <curl/curl.h>
#include <sys/types.h>
#include <sys/time.h>
#include "../includes/miniupnp/miniwget.h"
#include "../includes/miniupnp/miniupnpc.h"
#include "../includes/miniupnp/upnpcommands.h"
#include "../includes/miniupnp/upnperrors.h"
#include "nonportable.h"
#include "../includes/uthash.h"
#include "../includes/cJSON.h"
#include "utils777.c"
#include "inet.c"
#include "mutex.h"
#include "utlist.h"
#include "nn.h"
#include "pubsub.h"
#include "pipeline.h"
#include "survey.h"
#include "reqrep.h"
#include "bus.h"
#include "pair.h"

#define SUPERNET_PORT 7777
#define LB_OFFSET 1
#define PUBGLOBALS_OFFSET 2
#define PUBRELAYS_OFFSET 3
#define SUPERNET_APIENDPOINT "tcp://127.0.0.1:7776"

#define nn_errstr() nn_strerror(nn_errno())

extern int32_t Debuglevel;

#define OFFSET_ENABLED (bundledflag == 0)
#ifndef MIN
#define MIN(x,y) (((x)<=(y)) ? (x) : (y))
#endif
#ifndef MAX
#define MAX(x,y) (((x)>=(y)) ? (x) : (y))
#endif
typedef int32_t (*ptm)(int32_t,char *args[]);
// nonportable functions needed in the OS specific directory
int32_t is_bundled_plugin(char *plugin);
int32_t portable_truncate(char *fname,long filesize);
void *map_file(char *fname,uint64_t *filesizep,int32_t enablewrite);
int32_t os_supports_mappedfiles();
char *os_compatible_path(char *str);
char *OS_rmstr();
int32_t OS_launch_process(char *args[]);
int32_t OS_getppid();
int32_t OS_getpid();
int32_t OS_waitpid(int32_t childpid,int32_t *statusp,int32_t flags);
char *nn_typestr(int32_t type);

// only OS portable functions in this file
#define portable_mutex_t struct nn_mutex
#define portable_mutex_init nn_mutex_init
#define portable_mutex_lock nn_mutex_lock
#define portable_mutex_unlock nn_mutex_unlock

struct queueitem { struct queueitem *next,*prev; };
typedef struct queue
{
	struct queueitem *list;
	portable_mutex_t mutex;
    char name[31],initflag;
} queue_t;

struct pingpong_queue
{
    char *name;
    queue_t pingpong[2],*destqueue,*errorqueue;
    int32_t (*action)();
    int offset;
};

struct sendendpoints { int32_t push,rep,pub,survey; };
struct recvendpoints { int32_t pull,req,sub,respond; };
struct biendpoints { int32_t bus,pair; };
struct allendpoints { struct sendendpoints send; struct recvendpoints recv; struct biendpoints both; };
union endpoints { int32_t all[sizeof(struct allendpoints) / sizeof(int32_t)]; struct allendpoints socks; };
struct db777_entry { UT_hash_handle hh; uint32_t allocsize,valuelen,valuesize,keylen:30,linked:1,dirty:1; uint8_t value[]; };

struct db777
{
    void *db,*asyncdb;
    portable_mutex_t mutex;
    struct db777_entry *table;
    int32_t reqsock,valuesize,matrixentries;
    uint32_t start_RTblocknum;
    void **matrix; char *dirty;
    char compression[8],dbname[32],name[16],coinstr[16],flags;
    void *ctl,*env; char namestr[32],restoredir[512],argspecialpath[512],argsubdir[512],restorelogdir[512],argname[512],argcompression[512],backupdir[512];
    uint8_t checkbuf[10000000];
};

struct env777
{
    char coinstr[16],subdir[64];
    void *ctl,*env,*transactions;
    struct db777 dbs[16];
    int32_t numdbs,needbackup,lastbackup,currentbackup,matrixentries;
    uint32_t start_RTblocknum;
};

struct kv777_info { char PATH[1024]; int32_t readyflag; }; extern struct kv777_info KV777;

#define DEFAULT_APISLEEP 100  // milliseconds
#define NUM_PLUGINTAGS 8192
struct applicant_info { uint64_t senderbits; uint32_t nonce; char startflag,lbendpoint[128],relayendpoint[128],globalendpoint[128]; };

struct SuperNET_info
{
    char WEBSOCKETD[1024],NXTAPIURL[1024],NXTSERVER[1024],DATADIR[1024],transport[16],BACKUPS[512],SERVICENXT[64];
    char myipaddr[64],myNXTacct[64],myNXTaddr[64],NXTACCT[64],NXTADDR[64],NXTACCTSECRET[8192],SERVICESECRET[8192],userhome[512],hostname[512];
    uint64_t my64bits; uint8_t myprivkey[32],mypubkey[32];
    uint32_t myipbits,nonces[512],numnonces; struct applicant_info *responses; cJSON *peersjson; char lbendpoint[128],relayendpoint[128],globalendpoint[128];
    int32_t usessl,ismainnet,Debuglevel,SuperNET_retval,APISLEEP,gatewayid,numgateways,readyflag,UPNP,iamrelay,disableNXT,NXTconfirms,automatch,PLUGINTIMEOUT,ppid,noncing,pullsock,mmapflag;
    uint16_t port,serviceport;
    uint64_t tags[NUM_PLUGINTAGS][3];
    struct env777 DBs;
    struct kv777 *PM,*rawPM,*channels,*alias,*NXTaccts,*services;
    struct dKV777 *relays;
    cJSON *argjson;
}; extern struct SuperNET_info SUPERNET;

struct coins_info
{
    int32_t num,readyflag,slicei;
    cJSON *argjson;
    struct coin777 **LIST;
    // this will be at the end of the plugins structure and will be called with all zeros to _init
}; extern struct coins_info COINS;

struct db777_info
{
    char PATH[1024],RAMDISK[1024];
    int32_t numdbs,readyflag;
    struct db777 *DBS[1024];
}; extern struct db777_info SOPHIA;

#define MAX_MGWSERVERS 16
struct MGW_info
{
    char PATH[1024],serverips[MAX_MGWSERVERS][64],bridgeipaddr[64],bridgeacct[64];
    uint64_t srv64bits[MAX_MGWSERVERS],issuers[64];
    int32_t M,numissuers,readyflag,port;
    union endpoints all;
    uint32_t numrecv,numsent;
}; extern struct MGW_info MGW;

#define MAX_RAMCHAINS 128
struct ramchain_info
{
    char PATH[1024],coins[MAX_RAMCHAINS][16],pullnode[64];
    double lastupdate[MAX_RAMCHAINS];
    union endpoints all;
    int32_t num,readyflag,fastmode,verifyspends;
    // this will be at the end of the plugins structure and will be called with all zeros to _init
}; extern struct ramchain_info RAMCHAINS;

struct peers_info
{
    int32_t readyflag;
}; extern struct peers_info PEERS;

struct subscriptions_info
{
    char **publications;
    int32_t readyflag,numpubs;
}; extern struct subscriptions_info SUBSCRIPTIONS;

struct InstantDEX_info
{
    int32_t readyflag;
}; extern struct InstantDEX_info INSTANTDEX;

#define MAX_SERVERNAME 128
struct relayargs
{
    //char *(*commandprocessor)(struct relayargs *args,uint8_t *msg,int32_t len);
    char name[16],endpoint[MAX_SERVERNAME];
    int32_t sock,type,bindflag,sendtimeout,recvtimeout;
};

#define CONNECTION_NUMBITS 10
struct endpoint { uint64_t ipbits:32,port:16,transport:2,nn:4,directind:CONNECTION_NUMBITS; };
struct _relay_info { int32_t sock,num,mytype,desttype; struct endpoint connections[1 << CONNECTION_NUMBITS]; };
struct direct_connection { char handler[16]; struct endpoint epbits; int32_t sock; };

struct relay_info
{
    struct _relay_info active;
    struct nn_pollfd pfd[16];
    int32_t readyflag,subclient,lbclient,lbserver,servicesock,pubglobal,pubrelays,numservers;
}; extern struct relay_info RELAYS;

void expand_epbits(char *endpoint,struct endpoint epbits);
struct endpoint calc_epbits(char *transport,uint32_t ipbits,uint16_t port,int32_t type);

void lock_queue(queue_t *queue);
void queue_enqueue(char *name,queue_t *queue,struct queueitem *item);
void *queue_dequeue(queue_t *queue,int32_t offsetflag);
int32_t queue_size(queue_t *queue);
struct queueitem *queueitem(char *str);
void free_queueitem(void *itemptr);

int upnpredirect(const char* eport, const char* iport, const char* proto, const char* description);

void *myaligned_alloc(uint64_t allocsize);
int32_t aligned_free(void *alignedptr);

void *portable_thread_create(void *funcp,void *argp);
void randombytes(unsigned char *x,long xlen);
double milliseconds(void);
void msleep(uint32_t milliseconds);
#define portable_sleep(n) msleep((n) * 1000)

int32_t getline777(char *line,int32_t max);
char *bitcoind_RPC(char **retstrp,char *debugstr,char *url,char *userpass,char *command,char *args);
uint16_t wait_for_myipaddr(char *ipaddr);
void process_userinput(char *line);

char *get_localtransport(int32_t bundledflag);
int32_t init_socket(char *suffix,char *typestr,int32_t type,char *_bindaddr,char *_connectaddr,int32_t timeout);
int32_t shutdown_plugsocks(union endpoints *socks);
int32_t nn_local_broadcast(int32_t pushsock,uint64_t instanceid,int32_t flags,uint8_t *retstr,int32_t len);
//char *poll_local_endpoints(int32_t *lenp,int32_t pullsock);
struct endpoint nn_directepbits(char *retbuf,char *transport,char *ipaddr,uint16_t port);
int32_t nn_directsend(struct endpoint epbits,uint8_t *msg,int32_t len);

void ensure_directory(char *dirname);
uint32_t is_ipaddr(char *str);

uint64_t calc_ipbits(char *ipaddr);
void expand_ipbits(char *ipaddr,uint64_t ipbits);
char *ipbits_str(uint64_t ipbits);
char *ipbits_str2(uint64_t ipbits);
struct sockaddr_in conv_ipbits(uint64_t ipbits);
uint32_t conv_domainname(char *ipaddr,char *domain);
int32_t ismyaddress(char *server);

void set_endpointaddr(char *transport,char *endpoint,char *domain,uint16_t port,int32_t type);
int32_t nn_portoffset(int32_t type);

char *plugin_method(int32_t sock,char **retstrp,int32_t localaccess,char *plugin,char *method,uint64_t daemonid,uint64_t instanceid,char *origargstr,int32_t len,int32_t timeout,char *tokenstr);
//char *nn_direct(char *ipaddr,uint8_t *data,int32_t len);
//char *nn_publish(uint8_t *data,int32_t len,int32_t nostr);
//char *nn_allrelays(uint8_t *data,int32_t len,int32_t timeoutmillis,char *localresult);
char *nn_loadbalanced(uint8_t *data,int32_t len);
char *relays_jsonstr(char *jsonstr,cJSON *argjson);
struct daemon_info *find_daemoninfo(int32_t *indp,char *name,uint64_t daemonid,uint64_t instanceid);
int32_t init_pingpong_queue(struct pingpong_queue *ppq,char *name,int32_t (*action)(),queue_t *destq,queue_t *errorq);
int32_t process_pingpong_queue(struct pingpong_queue *ppq,void *argptr);
uint8_t *replace_forwarder(char *pluginbuf,uint8_t *data,int32_t *datalenp);
int32_t nn_socket_status(int32_t sock,int32_t timeoutmillis);

char *nn_busdata_processor(uint8_t *msg,int32_t len);
void busdata_init(int32_t sendtimeout,int32_t recvtimeout,int32_t firstiter);
int32_t busdata_poll();
char *busdata_sync(uint32_t *noncep,char *jsonstr,char *broadcastmode,char *destNXTaddr);
int32_t parse_ipaddr(char *ipaddr,char *ip_port);
int32_t construct_tokenized_req(uint32_t *noncep,char *tokenized,char *cmdjson,char *NXTACCTSECRET,char *broadcastmode);
int32_t validate_token(char *forwarder,char *pubkey,char *NXTaddr,char *tokenizedtxt,int32_t strictflag);
uint32_t nonce_func(int32_t *leveragep,char *str,char *broadcaststr,int32_t maxmillis,uint32_t nonce);
int32_t nonce_leverage(char *broadcaststr);
char *get_broadcastmode(cJSON *json,char *broadcastmode);
cJSON *serviceprovider_json();
int32_t nn_createsocket(char *endpoint,int32_t bindflag,char *name,int32_t type,uint16_t port,int32_t sendtimeout,int32_t recvtimeout);
int32_t nn_lbsocket(int32_t maxmillis,int32_t port,uint16_t globalport,uint16_t relaysport);
int32_t OS_init();
int32_t nn_settimeouts(int32_t sock,int32_t sendtimeout,int32_t recvtimeout);
int32_t is_duplicate_tag(uint64_t tag);
void portable_OS_init();

#define MAXTIMEDIFF 60

#endif
#else
#ifndef crypto777_system777_c
#define crypto777_system777_c

#ifndef crypto777_system777_h
#define DEFINES_ONLY
#include "system777.c"
#undef DEFINES_ONLY
#endif

struct nn_clock
{
    uint64_t last_tsc;
    uint64_t last_time;
} Global_timer;

double milliseconds()
{
    uint64_t nn_clock_now (struct nn_clock *self);
    return(nn_clock_now(&Global_timer));
}

void msleep(uint32_t milliseconds)
{
    void nn_sleep (int milliseconds);
    nn_sleep(milliseconds);
}

typedef void (portable_thread_func)(void *);
struct nn_thread
{
    portable_thread_func *routine;
    void *arg;
    void *handle;
};
void nn_thread_init (struct nn_thread *self,portable_thread_func *routine,void *arg);
void nn_thread_term (struct nn_thread *self);

static uint64_t _align16(uint64_t ptrval) { if ( (ptrval & 15) != 0 ) ptrval += 16 - (ptrval & 15); return(ptrval); }

void *myaligned_alloc(uint64_t allocsize)
{
    void *ptr,*realptr;
    realptr = calloc(1,allocsize + 16 + sizeof(realptr));
    ptr = (void *)_align16((uint64_t)realptr + sizeof(ptr));
    memcpy((void *)((long)ptr - sizeof(realptr)),&realptr,sizeof(realptr));
    printf("aligned_alloc(%llu) realptr.%p -> ptr.%p, diff.%ld\n",(long long)allocsize,realptr,ptr,((long)ptr - (long)realptr));
    return(ptr);
}

int32_t aligned_free(void *ptr)
{
    void *realptr;
    long diff;
    if ( ((long)ptr & 0xf) != 0 )
    {
        printf("misaligned ptr.%p being aligned_free\n",ptr);
        return(-1);
    }
    memcpy(&realptr,(void *)((long)ptr - sizeof(realptr)),sizeof(realptr));
    diff = ((long)ptr - (long)realptr);
    if ( diff < (long)sizeof(ptr) || diff > 32 )
    {
        printf("ptr %p and realptr %p too far apart %ld\n",ptr,realptr,diff);
        return(-2);
    }
    //printf("aligned_free: ptr %p -> realptr %p %ld\n",ptr,realptr,diff);
    free(realptr);
    return(0);
}

void *portable_thread_create(void *funcp,void *argp)
{
    //void nn_thread_term(struct nn_thread *self);
    void nn_thread_init(struct nn_thread *self,portable_thread_func *routine,void *arg);
    struct nn_thread *ptr;
    ptr = (struct nn_thread *)malloc(sizeof(*ptr));
    nn_thread_init(ptr,(portable_thread_func *)funcp,argp);
    return(ptr);
}

struct queueitem *queueitem(char *str)
{
    struct queueitem *item = calloc(1,sizeof(struct queueitem) + strlen(str) + 1);
    strcpy((char *)((long)item + sizeof(struct queueitem)),str);
    return(item);
}

void free_queueitem(void *itemptr) { free((void *)((long)itemptr - sizeof(struct queueitem))); }

void lock_queue(queue_t *queue)
{
    if ( queue->initflag == 0 )
    {
        portable_mutex_init(&queue->mutex);
        queue->initflag = 1;
    }
	portable_mutex_lock(&queue->mutex);
}

void queue_enqueue(char *name,queue_t *queue,struct queueitem *item)
{
    if ( queue->list == 0 && name != 0 && name[0] != 0 )
        safecopy(queue->name,name,sizeof(queue->name));
    if ( item == 0 )
    {
        printf("FATAL type error: queueing empty value\n"), getchar();
        return;
    }
    lock_queue(queue);
    DL_APPEND(queue->list,item);
    portable_mutex_unlock(&queue->mutex);
    //printf("name.(%s) append.%p list.%p\n",name,item,queue->list);
}

void *queue_dequeue(queue_t *queue,int32_t offsetflag)
{
    struct queueitem *item = 0;
    lock_queue(queue);
    if ( queue->list != 0 )
    {
        item = queue->list;
        DL_DELETE(queue->list,item);
        //printf("name.(%s) dequeue.%p list.%p\n",queue->name,item,queue->list);
    }
	portable_mutex_unlock(&queue->mutex);
    if ( item != 0 && offsetflag != 0 )
        return((void *)((long)item + sizeof(struct queueitem)));
    else return(item);
}

int32_t queue_size(queue_t *queue)
{
    int32_t count = 0;
    struct queueitem *tmp;
    lock_queue(queue);
    DL_COUNT(queue->list,tmp,count);
    portable_mutex_unlock(&queue->mutex);
	return count;
}

int32_t init_pingpong_queue(struct pingpong_queue *ppq,char *name,int32_t (*action)(),queue_t *destq,queue_t *errorq)
{
    ppq->name = name;
    ppq->destqueue = destq;
    ppq->errorqueue = errorq;
    ppq->action = action;
    ppq->offset = 1;
    return(queue_size(&ppq->pingpong[0]) + queue_size(&ppq->pingpong[1]));  // init mutex side effect
}

// seems a bit wastefull to do all the two iter queueing/dequeuing with threadlock overhead
// however, there is assumed to be plenty of CPU time relative to actual blockchain events
// also this method allows for adding of parallel threads without worry
int32_t process_pingpong_queue(struct pingpong_queue *ppq,void *argptr)
{
    int32_t iter,retval,freeflag = 0;
    void *ptr;
    //printf("%p process_pingpong_queue.%s %d %d\n",ppq,ppq->name,queue_size(&ppq->pingpong[0]),queue_size(&ppq->pingpong[1]));
    for (iter=0; iter<2; iter++)
    {
        while ( (ptr= queue_dequeue(&ppq->pingpong[iter],ppq->offset)) != 0 )
        {
            if ( Debuglevel > 2 )
                printf("%s pingpong[%d].%p action.%p\n",ppq->name,iter,ptr,ppq->action);
            retval = (*ppq->action)(&ptr,argptr);
            if ( retval == 0 )
                queue_enqueue(ppq->name,&ppq->pingpong[iter ^ 1],ptr);
            else if ( ptr != 0 )
            {
                if ( retval < 0 )
                {
                    if ( Debuglevel > 0 )
                        printf("%s iter.%d errorqueue %p vs %p\n",ppq->name,iter,ppq->errorqueue,&ppq->pingpong[0]);
                    if ( ppq->errorqueue == &ppq->pingpong[0] )
                        queue_enqueue(ppq->name,&ppq->pingpong[iter ^ 1],ptr);
                    else if ( ppq->errorqueue != 0 )
                        queue_enqueue(ppq->name,ppq->errorqueue,ptr);
                    else freeflag = 1;
                }
                else if ( ppq->destqueue != 0 )
                {
                    if ( Debuglevel > 0 )
                        printf("%s iter.%d destqueue %p vs %p\n",ppq->name,iter,ppq->destqueue,&ppq->pingpong[0]);
                    if ( ppq->destqueue == &ppq->pingpong[0] )
                        queue_enqueue(ppq->name,&ppq->pingpong[iter ^ 1],ptr);
                    else if ( ppq->destqueue != 0 )
                        queue_enqueue(ppq->name,ppq->destqueue,ptr);
                    else freeflag = 1;
                }
                if ( freeflag != 0 )
                {
                    if ( ppq->offset == 0 )
                        free(ptr);
                    else free_queueitem(ptr);
                }
            }
        }
    }
    return(queue_size(&ppq->pingpong[0]) + queue_size(&ppq->pingpong[0]));
}

uint16_t wait_for_myipaddr(char *ipaddr)
{
    uint16_t port = 0;
    printf("need a portable way to find IP addr\n");
    getchar();
    return(port);
}

int32_t ismyaddress(char *server)
{
    char ipaddr[64]; uint32_t ipbits;
    if ( strncmp(server,"tcp://",6) == 0 )
        server += 6;
    else if ( strncmp(server,"ws://",5) == 0 )
        server += 5;
    if ( (ipbits= is_ipaddr(server)) != 0 )
    {
        if ( strcmp(server,SUPERNET.myipaddr) == 0 || calc_ipbits(SUPERNET.myipaddr) == ipbits )
        {
            printf("(%s) MATCHES me (%s)\n",server,SUPERNET.myipaddr);
            return(1);
        }
    }
    else
    {
        if ( SUPERNET.hostname[0] != 0 && strcmp(SUPERNET.hostname,server) == 0 )
            return(1);
        if ( conv_domainname(ipaddr,server) == 0 && (strcmp(SUPERNET.myipaddr,ipaddr) == 0 || strcmp(SUPERNET.hostname,ipaddr) == 0) )
            return(1);
    }
    //printf("(%s) is not me (%s)\n",server,SUPERNET.myipaddr);
    return(0);
}

char *get_localtransport(int32_t bundledflag) { return(OFFSET_ENABLED ? "ipc" : "inproc"); }

int32_t nn_local_broadcast(int32_t sock,uint64_t instanceid,int32_t flags,uint8_t *retstr,int32_t len)
{
    int32_t i,sendlen,errs = 0;
    if ( sock >= 0 )
    {
        for (i=0; i<10; i++)
            if ( (nn_socket_status(sock,1) & NN_POLLOUT) != 0 )
                break;
        if ( (sendlen= nn_send(sock,(char *)retstr,len,0)) <= 0 )
            errs++, printf("sending to socket.%d sendlen.%d len.%d (%s) [%s]\n",sock,sendlen,len,nn_strerror(nn_errno()),retstr);
        else if ( Debuglevel > 2 )
            printf("nn_local_broadcast SENT.(%s) len.%d sendlen.%d vs strlen.%ld instanceid.%llu -> sock.%d\n",retstr,len,sendlen,strlen((char *)retstr),(long long)instanceid,sock);
    }
    return(errs);
}

int32_t plugin_result(char *retbuf,cJSON *json,uint64_t tag)
{
    char *error,*result;
    error = cJSON_str(cJSON_GetObjectItem(json,"error"));
    result = cJSON_str(cJSON_GetObjectItem(json,"result"));
    if ( error != 0 || result != 0 )
    {
        sprintf(retbuf,"{\"result\":\"completed\",\"tag\":\"%llu\"}",(long long)tag);
        return(1);
    }
    return(0);
}

double estimate_completion(double startmilli,int32_t processed,int32_t numleft)
{
    double elapsed,rate;
    if ( processed <= 0 )
        return(0.);
    elapsed = (milliseconds() - startmilli);
    rate = (elapsed / processed);
    if ( rate <= 0. )
        return(0.);
    //printf("numleft %d rate %f\n",numleft,rate);
    return(numleft * rate);
}

void clear_alloc_space(struct alloc_space *mem,int32_t alignflag)
{
    memset(mem->ptr,0,mem->size);
    mem->used = 0;
    mem->alignflag = alignflag;
}

void rewind_alloc_space(struct alloc_space *mem,int32_t flags)
{
    if ( (flags & 1) != 0 )
        clear_alloc_space(mem,1);
    else mem->used = 0;
    mem->alignflag = ((flags & ~1) != 0);
}

struct alloc_space *init_alloc_space(struct alloc_space *mem,void *ptr,long size,int32_t flags)
{
    if ( mem == 0 )
        mem = calloc(1,size + sizeof(*mem)), ptr = mem->space;
    mem->size = size;
    mem->ptr = ptr;
    rewind_alloc_space(mem,flags);
    return(mem);
}

void *memalloc(struct alloc_space *mem,long size,int32_t clearflag)
{
    void *ptr = 0;
    if ( (mem->used + size) > mem->size )
    {
        printf("alloc: (mem->used %ld + %ld size) %ld > %ld mem->size\n",mem->used,size,(mem->used + size),mem->size);
        while ( 1 )
            portable_sleep(1);
    }
    ptr = (void *)((long)mem->ptr + mem->used);
    mem->used += size;
    if ( clearflag != 0 )
        memset(ptr,0,size);
    if ( mem->alignflag != 0 && (mem->used & 0xf) != 0 )
        mem->used += 0x10 - (mem->used & 0xf);
    return(ptr);
}

void portable_OS_init()
{
    void SaM_PrepareIndices();
    OS_init();
    curl_global_init(CURL_GLOBAL_ALL); //init the curl session
    SaM_PrepareIndices();
}
#endif
#endif
