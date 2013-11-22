//#include "hashfunc.h"

#define SMALL 16
#define MIDDLE 256
#define BUFSIZE 8192 //8K
#define BIG 65536

#undef  DEBUG
//#define DEBUG_MSG2 fprintf(stderr,"debug:%s,%d\n",__FILE__,__LINE__);
#ifdef DEBUG
#define DEBUG_MSG(msg) fprintf(stderr,"debug:[%s:%d] %s\n",__FILE__,__LINE__,msg);
#else
#define DEBUG_MSG(msg) ;
#endif

#define FALSE 0
#define TRUE 1
#define VERSION "0.1.1"

#define prterrmsg(msg) {fprintf(stderr,msg);}
#define wrterrmsg(msg) {write(error_log_fd,msg,strlen(msg));}
#define log_access(msg){write(access_log_fd,msg,strlen(msg));}
#define prtinfomsg(msg) {fputs(msg, stdout); }
#define wrtinfomsg(msg) {write(error_log_fd,msg,strlen(msg));}

#define HL_NEW_QUEUE(x) \
    x=malloc(sizeof(hl_queue));\
    x->tail=NULL; \
    x->head=NULL;
#define HL_NEW_QUEUE_ITEM(x) \
        x=malloc(sizeof(hl_queue_item));
#define HL_INIT_QUEUE_ITEM(x,init) \
        x=malloc(sizeof(hl_queue_item)); \
        x->data=(void * ) init;
#define HL_RESET_POOL(x) \
    x->total_size=0; \
    x->head=NULL;\
    x->using_pool=NULL;\
    x->tail=NULL;\
    x->current_size=0;\
    x->chain=NULL;\
    x->larges=NULL;\
    x->next=NULL; \
    x->error_times=0;\
    x->root = NULL;

#define DEBUG_MEM 0
#define HL_ALLOC(p,size) p=malloc((int)size); \
    if(DEBUG_MEM) printf("ALLOC LOG:malloc at:%s#%d,size:%lu,pointer:%p\n",__FILE__,__LINE__,size,p);
#define HL_CALLOC(p,type,num) p=(type * )calloc(sizeof(type),num); \
    if(DEBUG_MEM) printf("ALLOC LOG:cmalloc at:%s#%d,size:%lu\n",__FILE__,__LINE__,sizeof(type)*num);

#define HL_FREE(x) if(x!=NULL) { \
    if(DEBUG_MEM) printf("FREE LOG:free at:%s#%d,pointer:%p\n",__FILE__,__LINE__,x);\
    free(x);\
    x=NULL;\
}
#define HL_LARGE_SIZE 8192


int cbstricmp(const char *astr, const char *bstr);
int cbstrfwimatch(const char *str, const char *key);
int cbstrbwmatch(const char *str, const char *key);


typedef struct {
        char **ptr;
        size_t size;
        size_t used;
} char_array;

 struct hl_string_t {
    size_t len;
    char * data;
} ;
typedef struct hl_string_t  hl_string;
 struct  hl_string_array_t {
    int size;
    int used;
    hl_string *p;
} ;
typedef struct hl_string_array_t hl_string_array;
struct hl_int_array_t {
    int size;
    int used;
    int ** p;
};
typedef struct hl_int_array_t hl_int_array;
struct hl_keyval_pair_t  {
    hl_string * key;
    hl_string * value;
} ;
typedef struct hl_keyval_pair_t  hl_keyval_pair;

struct hl_list_t {
    int size;
    int pos;    //当前位置
    void * * p;
};
typedef struct hl_list_t hl_list;
typedef struct hl_list_t hl_dict;

struct response_arg_t {
    int fd;
    char * fpath;
    int thread_id;
    hl_string *root;
    int thread_no;
} ;
typedef struct response_arg_t response_arg;

struct hl_thread_node_t {
//#pthread_t  thread;
    int active;
    int create_res;
};
typedef  struct hl_thread_node_t hl_thread_node;

struct hl_queue_item_t {
    int fd;
    struct hl_queue_item_t * next;
    struct hl_queue_item_t * prev;
};
typedef struct hl_queue_item_t hl_queue_item;

struct hl_queue_t {
    hl_queue_item * head;
    hl_queue_item * tail;
};
typedef struct hl_queue_t hl_queue;


struct hl_request_t {
    int sock_fd;
};
typedef struct hl_request_t hl_request;

struct hl_response_t {
    int sock_fd;
};
typedef struct hl_response_t hl_response;


struct hl_pool_t {
    size_t total_size;
    char * head;
    char * last;
    char * tail;
    size_t current_size;
    struct hl_pool_t * chain;
    struct hl_pool_t  * larges;
    struct hl_pool_t * next;
    struct hl_pool_t * using_pool;
    int error_times;
    struct hl_pool_t * root;
};
typedef struct hl_pool_t hl_pool;

struct s_hl_link{
    struct s_hl_link * head;
    struct s_hl_link * next;
    struct s_hl_link * prev;
    struct s_hl_link * tail;
    void * data;
    char * key;
};
typedef struct s_hl_link hl_link;



#define hl_link_member \
struct s_hl_link * head;\
struct s_hl_link * next;\
struct s_hl_link * prev;\
struct s_hl_link * tail;\
void * data;

#define hl_list_at(link,index,return_data) \
    if(link!=NULL) { \
        if(index>=0 && index<= link->pos) {\
            return_data =  link->p[index];   \
        }\
    }

#define hl_list_set_at(link,index,data) \
    if(link!=NULL) { \
        if(index>=0 && index<= link->pos) {\
              link->p[index]= data;   \
        }\
    }


#define hl_link_init(link) \
{\
    link->head = link; \
    link->tail = link;\
    link->prev = NULL;\
    link->next = NULL;\
    link->data = NULL;\
}

#define hl_new_link(link)\
    hl_link * link=NULL;\
    link = HL_CALLOC(link,hl_link,1);\
    if(link!=NULL){\
        hl_link_init(link);\
    }
#define hl_free_link(link)\
        HL_FREE(link);

#define hl_link_append(current,new_item)\
    new_item->prev = current;\
    current->next = new_item;\
    new_item->head = current->head;\
    current->head->tail = new_item;\
    current->tail = new_item;
    //current->tail->

#define hl_link_iter(root,current)    for(current = root->head;current!=NULL;current= current->next)

#define hl_link_iter_r(root,current)    for(current = root->tail;current!=NULL;current= current->prev)

#define hl_string_free(str)  \
{ \
    if(str!=NULL) {\
        hl_free(  str->data); \
        hl_free( str); \
    str = NULL;\
    } \
}

#define  hl_free(ptr)  \
{ \
    if(ptr!=NULL){ \
        free((void * )ptr); \
        ptr = NULL; \
    } \
}

#define hl_hashtable_set(tbl,key,val) \
        unsigned int rkey = HASH_FUNC(key);
#define hl_new_hashtable hl_new_list 
#define hl_new_root_pool(root_pool,size) \
    HL_RESET_POOL(root_pool);\
    assert(hl_prepare_pool(root_pool,size));\
    root_pool->root = root_pool;\
    root_pool->using_pool = root_pool;\
    root_pool->next = NULL;

/**
 * @return 1:value replaced
 * @return 2:value added
 */




size_t hl_fill_string(hl_string * p,const char * data);
hl_string *  hl_new_string(char * p);
size_t hl_fill_string(hl_string * p,const char * data);

hl_list * hl_new_list(int size);
int hl_list_append(hl_list * p,void * data);
int hl_list_is_bof(hl_list * p);
int hl_list_is_empty(hl_list * p );
int hl_list_is_eof(hl_list * p);
int hl_list_shift();
void hl_list_free(hl_list * list);
void *  hl_list_pop(hl_list  * list);
hl_pool * hl_prepare_pool(hl_pool * pool,size_t size);
void * hl_pool_alloc(hl_pool * pool,size_t size);

int hl_ht_set(hl_list * ht,int bucket_size,char *key,void * data);
hl_link * hl_ht_get(hl_list *ht,int bucket_size,char *key);
