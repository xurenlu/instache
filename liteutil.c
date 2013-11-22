#include "stdlib.h"
#include "string.h"
#include "liteutil.h"
#include "stdio.h"
#include <assert.h>
#include "hashfunc.h"

#define HASH_FUNC BKDRHash
char e404[]="<h1>404 Error</h1><hr><p>File not found!</p>";
char e403[]="<h1>403 Error</h1><hr><p>Fobidden!</p>";
char e50x[]="<h1>500 Error</h1><hr><p>Internel Error!</p>";

void * hl_pool_alloc(hl_pool * pool,size_t size);
//
//
/**
 * 初始化一个字符串结构,指定内容为空,长度为0;
 **/
hl_string *  hl_init_string(hl_string * p){
    //    hl_string * p;
    // p=calloc(sizeof(hl_string),1);
    p->len=0;
    p->data=NULL;
    return p;
}
/**
 * malloc address for the string and set value ;
 * */
size_t hl_fill_string(hl_string * p,const char * data){
    p->len=(int)strlen(data)+1;
    p->data=malloc(p->len*sizeof(char));
    bzero(p->data,p->len);
    memcpy(p->data,data,p->len);
    return p->len;
}
/**
 * generate hl_string struct from a char point;
 */
hl_string *  hl_new_string(char * p){
    hl_string * str;
    str=calloc(sizeof(hl_string),1);
    hl_string * return_str=hl_init_string(str);
    hl_fill_string(return_str,p);
    return return_str;
}

/**
 * free the string memory
 * */


/**
 * generate new LIST */
hl_list * hl_new_list(int size){
    hl_list * list;
    list=(hl_list *) malloc(sizeof(hl_list));
    
    list->size=size;
    list->pos=0;
    list->p=malloc(sizeof(void *)*size);
    return list;
}
/**
 * append new data to the end of the pointer
 * */
int hl_list_append(hl_list * p,void * data){
    /**
     * handle when pos<size...
     * */
    if(p->pos < p->size ) {
        p->p[p->pos]=data;
        p->pos++;
        return p->pos;
    }else{
        return -1;//means that ,list is full.
    }
}
/**
 * check if reach the head of the list;
 */
int hl_list_is_bof(hl_list * p){
    if(p->pos==0)
    {
        return 1;
    }
    else
        return 0;
}
/**
 * check  if the list is empty
 * */
int hl_list_is_empty(hl_list * p ){
    return hl_list_is_bof(p);
}
/**
 * check if is the tail of the list
 * */
int hl_list_is_eof(hl_list * p){
    if(p->pos==p->size)
        return 1;
    else
        return 0;
}
/**
 * pop out the last item of the list , delete it from the list;
 * return NULL if we have reached the end;
 */
void *  hl_list_pop(hl_list  * list){
    void * p=NULL;
    if(list->pos>0){
        list->pos--;
        p=list->p[list->pos];
        if(p==NULL){
            return NULL;
        }
        return p;
    }
    else{
        return NULL;
    }
}
/**
 * 释放list资源
 */
void hl_list_free(hl_list * list){
    hl_free(list);
}
/**
 * split a string to list
 * please call this function twice
 * first with third argument NULL,then you can got a number:the size of the list
 * then,call it again,and pass it with third argument a hl_list variable with the known size;
 * @param int just_first:return when the [separator] first found.
 * */
int hl_split(hl_string * separator,hl_string * str,hl_list * list,int just_first){
    assert(separator->data);
    /**
     * when list == NULL,will return the size of new list
     */
    hl_string * newstr;
    int i=0;
    int j=0;
    int equal=1;
    char * pj;
    char * temp;
    pj=malloc(6);
    memcpy(pj,"hello",5);
    size_t sep_len=separator->len;
    size_t str_len=str->len;
    int last_pos=0;
    int return_array_size=0;
    for(i=0;i<(str_len-sep_len);i++){
        equal=1;
        for(j=0;j<sep_len-1;j++){
            if(str->data[i+j]!=separator->data[j]){
                equal=0;
                break;
            }
        }
        if(equal==1){
            return_array_size++;
            if(list!=NULL){
                temp=str->data+i+j;
                pj=malloc( (i-last_pos)*sizeof(char));
                memcpy(pj,str->data+last_pos,(i-last_pos)*sizeof(char));
                newstr=hl_new_string(pj);
                hl_list_append(list,newstr);
                last_pos=i+j;
            }
        }
        if(equal==1 && just_first){
            equal=0;
            break;
        }
        
    }
    if(equal==0){
        return_array_size++;
        if(list!=NULL){
            temp=str->data+last_pos+1;
            pj=malloc( (str->len-last_pos)*sizeof(char));
            memcpy(pj,str->data+last_pos,(str->len-last_pos)*sizeof(char));
            newstr=hl_new_string(pj);
            hl_list_append(list,newstr);
        }
        return return_array_size;
    }
    else{
        return return_array_size;
    }
    
    
}
/**
 * generate a key-value-pair point
 * */
hl_keyval_pair * hl_init_keyval_pair(){
    
    hl_keyval_pair * kvpair;
    //HL_ALLOC(kvpair, sizeof(hl_keyval_pair));
    HL_CALLOC(kvpair, hl_keyval_pair, 1);
    kvpair->key =NULL;
    kvpair->value = NULL;
    return kvpair;
    
}
/**
 * set the key and value of a key-value pair ;
 * */
int hl_set_keyval_pair(hl_keyval_pair * kvpair,hl_string * key,hl_string * val){
    hl_string * keystr;
    hl_string * valstr;
    
    keystr=hl_new_string("");
    valstr=hl_new_string("");
    
    hl_fill_string(keystr,key->data);
    hl_fill_string(valstr,val->data);
    kvpair->key=keystr;
    kvpair->value=valstr;
    return 1;
}
/**
 * init a key-value-pair with specificed key and value
 * */
hl_keyval_pair * hl_init_keyval_pair_withkv(hl_string * key,hl_string * val){
    int ret=-1;
    hl_keyval_pair * kvpair;
    kvpair=hl_init_keyval_pair();
    if(!kvpair) return NULL;
    ret=hl_set_keyval_pair(kvpair,key,val);
    if(ret<0)
        return NULL;
    else
        return kvpair;
}


/**
 * free the memory of the key-value-pair
 * It didn't release the memory of the key and value;
 * */
void hl_free_keyval_pair(hl_keyval_pair * kvpair){
    hl_free(kvpair);
}


/**
 * set the value of specificed key of a dict.
 * */
void hl_dict_set(hl_dict * dict,hl_string * key,hl_string * val){
    hl_keyval_pair * pair;
    int i=0;
    for(;i<dict->pos;i++){
        pair=(hl_keyval_pair * )dict->p[i];
        if(!strcmp(pair->key->data,key->data))
        {
            pair->value=val;
            return;
        }
    }
    pair=hl_init_keyval_pair_withkv(key,val);
    hl_list_append(dict,pair);
}

/**
 * set the value of specificed key of a dict,parameters are chars
 * */
void hl_dict_set_by_chars(hl_dict * dict,char * key,char * val){
    hl_string * keystring;
    hl_string * valstring;
    keystring=hl_new_string(key);
    valstring=hl_new_string(val);
    hl_dict_set(dict,keystring,valstring);
    hl_string_free(keystring);
}
/**
 * return that if the dict has the specified key
 **/
int hl_dict_has_key (hl_dict * dict,hl_string * key){
    hl_keyval_pair * pair;
    int i=0;
    for(;i<dict->pos;i++){
        pair=(hl_keyval_pair * )dict->p[i];
        if(!strcmp(pair->key->data,key->data))
        {
            return 1;
        }
    }
    return 0;
}

/**
 * return the value of specificed key of a dict
 * if there is no item with specified key, you would get a string ""
 * */
hl_string * hl_dict_get(hl_dict * dict,hl_string * key){
    hl_keyval_pair * pair;
    int i=0;
    for(;i<dict->pos;i++){
        pair=(hl_keyval_pair * )dict->p[i];
        if(!strcmp(pair->key->data,key->data))
        {
            return pair->value;
        }
    }
    return NULL;
}

/**
 * get dict data using "const char * key " as key.
 *
 * */
hl_string * hl_dict_get_by_chars(hl_dict * dict , char  * key){
    hl_string * return_str;
    hl_string * key_str;
    key_str= hl_new_string(key);
    //    hl_fill_string(key_str,key);
    return_str =  hl_dict_get(dict,key_str);
    hl_string_free(key_str);
    return return_str;
}
/**
 * return that if the dict has the specified value
 **/
int hl_dict_has_val (hl_dict * dict,hl_string * val){
    hl_keyval_pair * pair;
    int i=0;
    for(;i<dict->pos;i++){
        pair=(hl_keyval_pair * )dict->p[i];
        if(!strcmp(pair->value->data,val->data))
        {
            return 1;
        }
    }
    return 0;
}

void hl_dict_free(hl_dict * dict){
    hl_keyval_pair * pair;
    int i=0;
    for(;i<dict->pos;i++){
        pair=(hl_keyval_pair * )dict->p[i];
        hl_free_keyval_pair(pair);
    }
}


/**
 * parse the ini config file;
 * @configfilepath 为配置文件路径
 * */
int  hl_parse_config_file(hl_string *  config_file,hl_dict * dict ){

    assert(dict);
    // size_t k=0;
    size_t len=0;
    char comment[2]="#";
    char buf[1024],key[1024],val[1024];//temp[1024];
    FILE * fd;
    char * configfilepath=config_file->data;
    
    hl_dict_set_by_chars(dict,"root","");
    hl_dict_set_by_chars(dict,"port","");
    hl_dict_set_by_chars(dict,"hostname","");
    hl_dict_set_by_chars(dict,"access_log","");
    hl_dict_set_by_chars(dict,"error_log","");
    hl_dict_set_by_chars(dict,"max_clients","");
    hl_dict_set_by_chars(dict,"max_childs","");
    hl_dict_set_by_chars(dict,"run_daemon","");
    hl_dict_set_by_chars(dict,"cgi_pattern","");
    hl_dict_set_by_chars(dict,"cgi_dir","");

    
    fd=fopen(configfilepath,"r");
    if(!fd){
        fprintf(stderr,"Can't open the file:%s\n",configfilepath);
    }
    int rt_value=0;
    int line_no=0;
    while(fgets(buf,1024,fd)){
        line_no++;
        //ignore the comment;
        if(cbstrfwimatch(buf, comment))
            continue;
        sscanf(buf,"%1024[^=]=%s",key,val);
        len=strlen(val);
        if(!cbstricmp(key,"port")){
            hl_dict_set_by_chars(dict,"port",
                                 val);
        }
        else if(!cbstricmp(key,"host")){
            hl_dict_set_by_chars(dict,"host",val);
        }
        else if(!cbstricmp(key,"root")){
            hl_dict_set_by_chars(dict,"root",val);
        }
        else if(!cbstricmp(key,"cgi_pattern")){
            hl_dict_set_by_chars(dict,"cgi_pattern",val);
        }
        else if(!cbstricmp(key,"access_log")){
            hl_dict_set_by_chars(dict,"access_log",val);
        }
        else if(!cbstricmp(key,"error_log")){
            hl_dict_set_by_chars(dict,"error_log",val);
        }
        else if(!cbstricmp(key,"max_clients")){
            hl_dict_set_by_chars(dict,"max_clients",val);
        }
        else if(!cbstricmp(key,"max_childs")){
            hl_dict_set_by_chars(dict,"max_childs",val);
        }
        else if(!cbstricmp(key,"run_daemon")){
            hl_dict_set_by_chars(dict,"run_daemon","y");
            if(!cbstricmp(val,"no")){
                hl_dict_set_by_chars(dict,"run_daemon","n");
            }
        }
        else if(!cbstricmp(key,"cgi_dir")){
            hl_dict_set_by_chars(dict,"cgi_dir",val);
        }
        else {
            rt_value=-1;
        }
    }
    fclose(fd);
    return rt_value;
}

void
hexdump(const void *buf, int len)
{
    int offset = 0;
    int line_offset;
    while (offset < len) {
        printf("%08X:", offset);
        for (line_offset = 0; line_offset < 16; line_offset++) {
            if ((line_offset % 4) == 0)
                printf(" ");
            if (offset + line_offset < len)
                printf(" %02X", ((unsigned char *) buf)[offset + line_offset]);
            else
                printf("   ");
        }
        printf("  ");
        for (line_offset = 0; line_offset < 16; line_offset++) {
            if (offset + line_offset >= len)
                break;
            // printf("%c", sprintf(((unsigned char *) buf)[offset + line_offset]) ?
            //((unsigned char *) buf)[offset + line_offset] : '.');
        }
        offset += 16;
        printf("\n");
    }
}
//void log_access(char * msg){  fputs(msg,access_log_fd);   fflush(access_log_fd);  }
/* Convert the letters of a string to lower case. */
char *cbstrtolower(char *str){
    int i;
    assert(str);
    for(i = 0; str[i] != '\0'; i++){
        if(str[i] >= 'A' && str[i] <= 'Z') str[i] += 'a' - 'A';
    }
    return str;
}
/* compare two strings with case insensitive evaluation. */
int cbstricmp(const char *astr, const char *bstr){
    int ac, bc;
    assert(astr && bstr);
    while(*astr != '\0'){
        if(*bstr == '\0') return 1;
        ac = (*astr >= 'A' && *astr <= 'Z') ? *astr + ('a' - 'A') : *(unsigned char *)astr;
        bc = (*bstr >= 'A' && *bstr <= 'Z') ? *bstr + ('a' - 'A') : *(unsigned char *)bstr;
        if(ac != bc) return ac - bc;
        astr++;
        bstr++;
    }
    return *bstr == '\0' ? 0 : -1;
}

/* Check whether a string begins with a key, with case insensitive evaluation. */
int cbstrfwimatch(const char *str, const char *key){
    int sc, kc;
    assert(str && key);
    while(*key != '\0'){
        if(*str == '\0') return FALSE;
        sc = *str;
        if(sc >= 'A' && sc <= 'Z') sc += 'a' - 'A';
        kc = *key;
        if(kc >= 'A' && kc <= 'Z') kc += 'a' - 'A';
        if(sc != kc) return FALSE;
        key++;
        str++;
    }
    return TRUE;
}
/* Check whether a string ends with a key. */
int cbstrbwmatch(const char *str, const char *key){
    size_t slen, klen, i;
    assert(str && key);
    slen = strlen(str);
    klen = strlen(key);
    for(i = 1; i <= klen; i++){
        if(i > slen || str[slen-i] != key[klen-i]) return FALSE;
    }
    return TRUE;
}

/**
 * allocate memory for a hl_thread_node;
 * */
hl_thread_node * hl_new_thread_node(){
    hl_thread_node * p= malloc(sizeof(hl_thread_node));
    p->active=0;
    return p;
}
/**
 * @param pool :
 * */
hl_pool * hl_prepare_pool(hl_pool * pool,size_t size){
    char * p;
    DEBUG_MSG("calling hl_new_pool");
    HL_ALLOC(p,size);
    if(p==NULL)
        return NULL;
    DEBUG_MSG("hl_new_pool:allocated");
    pool->total_size=size;
    pool->head=pool->last=p;
    pool->tail=p+size;
    pool->using_pool=pool;
    return pool;
}

/** alloc large memeory space ,and asign it to the pool  */
void * hl_pool_alloc_large(hl_pool * pool,size_t size){
    assert(pool);
    if(pool->larges==NULL){
        pool->larges=hl_pool_alloc(pool,sizeof(hl_pool));
        HL_RESET_POOL(pool->larges);
    }
    if(pool->larges==NULL){
        return NULL;
    }
    hl_pool * new;
    new=hl_pool_alloc(pool,sizeof(hl_pool));
    DEBUG_MSG("hl_pool_alloc_large called");
    if(new==NULL){
        DEBUG_MSG("alloc mem for new larges failed.");
        return NULL;
    }
    //    DEBUG_MSG(")
    HL_RESET_POOL(new);
    assert(hl_prepare_pool(new,size));
    hl_pool * p;
    p=pool->larges;
    while(p->next!=NULL){
        p=p->next;
    }
    p->next=new;
    new->root = pool;
    new->current_size  = size;
    return new->head;
}
/**
 * 重置 pool,这之前请记得已经释放了全部的large
 * */
void  hl_pool_reset(hl_pool * pool){
    if(pool==NULL) return ;
    hl_pool * pt;
    if(pool->using_pool!=NULL){
        for(pt=pool->using_pool;pt!=NULL;pt=pt->next){
            HL_FREE(pt->head);
            HL_RESET_POOL(pt);
        }
    }
    HL_FREE(pool->head);
    HL_RESET_POOL(pool);
}

/**
 * 释放pool中所有的larges
 * */
void hl_pool_clear(hl_pool * pool){
    if(pool==NULL) return ;
    hl_pool * p;
    p=pool->larges;
    if(p!=NULL){
        HL_FREE(p->head);
        while(p->next!=NULL){
            p=p->next;
            if(p==NULL){
                break;
            }else{
                HL_FREE(p->head);
            }
        }
    }
    hl_pool_reset(pool);
}

/**
 * alloc memory from the pool;
 */
void * hl_pool_alloc(hl_pool * pool,size_t size){
    if(pool==NULL) return NULL;
    void * p;
    hl_pool * pt;
    if (size>HL_LARGE_SIZE){
        //alloc a large block,and add it to the large chains;
        return hl_pool_alloc_large(pool,size);
    }
    assert(pool->using_pool);
    /*
     * find from the pool chains
     */
     for(pt=pool->using_pool;pt!=NULL;pt=pt->next)
        {
            if(pt->error_times< 8 && size<(pt->total_size-pt->current_size)){
                //alloc from the pool
                p=pt->last+size;
                pt->last=p;
                pt->current_size+=size;
                return p;
            }
            else{
                if(pt->error_times<8){
                    //这个分支就是此池的内存大小不够的情况;
                    pt->error_times++;
                }
                //exit(1);
            }
        }
        //     DEBUG_MSG

    //    DEBUG_MSG
    /** if we can't find space from pools,we create new one
     and add to the "next" */
    hl_pool * new;
    new=hl_pool_alloc(pool,sizeof(hl_pool));
    if(new==NULL){
        HL_ALLOC(new,sizeof(hl_pool));
    }
    if(new==NULL) return NULL;
    //    DEBUG_MSG
    HL_RESET_POOL(new); 
    assert(hl_prepare_pool(new,2 * pool->using_pool->total_size));
    new->next=pool->using_pool;
    new->root = pool;
    pool->using_pool=new;
//    pool->total_size
    return hl_pool_alloc(pool,size);
//    return pool;
}



/**
 * @return 1:value replaced
 * @return 2:value added
 */
 inline int hl_ht_set(hl_list * ht,int bucket_size,char *key_param,void * data_param){
    //assert(bucket_size>0 && key_param!=NULL && data_param!=NULL);
    void * fetched_data=NULL;
    hl_link * link=NULL;
    hl_link * current=NULL;
    int hash_key = HASH_FUNC(key_param) % bucket_size;
    // printf("\n\ngot hash_key:%d",hash_key);
    hl_list_at(ht,hash_key,fetched_data);
    if(fetched_data==NULL){
        HL_CALLOC(link, hl_link, 1);
        hl_link_init(link);
        link->data = data_param;
        link->key = key_param;
        hl_list_set_at(ht,hash_key,link);
        return 2;
    }else{
        link = (hl_link * )fetched_data;
        hl_link_iter(link,current){
            if(current!=NULL && strcmp(key_param,current->key)==0){
                //replace it;
                HL_FREE(current->data);
                current->data = data_param;
                return 1;
            }
        }
        HL_CALLOC(current, hl_link, 1);
        if(current==NULL){
            return -1;
        }
        hl_link_init(current);
        current->data = data_param;
        current->key = key_param;
        hl_link_append(link->tail, current);
        return 2;
    }
}

hl_link * hl_ht_get(hl_list *ht,int bucket_size,char *key){
    void * tmp_data=NULL;
    hl_link * item;
    hl_link * current=NULL;
    char * str=NULL;
    hl_list_at(ht,(HASH_FUNC(key) % bucket_size ),tmp_data);
    if(tmp_data !=NULL){
        item = (hl_link * ) tmp_data;
        hl_link_iter(item, current){
            str = (char *)current->data;
            if(str!=NULL && current->key!=NULL){
                if(strcmp(key,current->key)==0){
                    return current;
                }
            }
        }
        return NULL;
    }
    return NULL;
}


