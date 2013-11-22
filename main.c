//
//  main.c
//  hlite
//
//  Created by xu renlu on 13-11-7.
//  Copyright (c) 2013年 xu renlu. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "liteutil.h"
#include "hashfunc.h"

#define BUCKET_SIZE 8

void test_string();
void test_link();
void test_hashtable();
void test_hashtable2();
void test_pool();
void test_pool_large();
//#define hl_ht_set
int main(int argc, const char * argv[])
{

    // insert code here...
    //printf("Hello, World!\n");
//    test_string();
    //test_link();
    //test_hashtable2();
//    test_pool();
//    test_pool_large();
    return 0;
}

void test_pool_large(){
    hl_pool * auto_pool;
    //    auto_pool = HL_CALLOC(<#p#>, <#type#>, <#num#>)
    HL_CALLOC(auto_pool, hl_pool, 1);
    HL_RESET_POOL(auto_pool);
    hl_prepare_pool(auto_pool,1024*1);
    char * p;
    p=hl_pool_alloc(auto_pool, 10240);
    if(p!=NULL){
        strcpy(p, "Hello world");
        printf("got string:%s\n",p);
    }
}
void test_pool(){
    hl_pool * auto_pool;
    //    auto_pool = HL_CALLOC(<#p#>, <#type#>, <#num#>)
    HL_CALLOC(auto_pool, hl_pool, 1);
    HL_RESET_POOL(auto_pool);
    hl_prepare_pool(auto_pool,1024*1);
    char * p =NULL;
    p = hl_pool_alloc(auto_pool,sizeof(char)*32);
    assert(p);
    strncpy(p,"Hello World",12);
    printf("got string:%s\n",p);
    
    char * p2 =NULL;
    p2 = hl_pool_alloc(auto_pool,sizeof(char)*32);
    assert(p2);
    strncpy(p2,"Welcome to Beijing",strlen("Welcome to Beijing")+1);
    printf("got string:%s\n",p2);
    printf("total_size:%d\n",(int)auto_pool->total_size);
    
    char * p3=NULL;
    p3 = hl_pool_alloc(auto_pool,sizeof(char)*1024);
    assert(p3);
    strncpy(p3,"Welcome to Hangzhou",strlen("Welcome to Hangzhou")+1);
    printf("p3:%s\n",p3);
    printf("using pool current_size:%d,total_size:%D\n",(int)auto_pool->using_pool->current_size,(int)auto_pool->using_pool->total_size);
    
    char * p4=NULL;
    p4 = hl_pool_alloc(auto_pool,sizeof(char)*1024);
    assert(p4);
    strncpy(p4,"Welcome to Nanjing",strlen("Welcome to Nanjing")+1);
    printf("p4:%s\n",p4);
    printf("using pool current_size:%d,total_size:%D\n",(int)auto_pool->using_pool->current_size,(int)auto_pool->using_pool->total_size);
    
    char * p5=NULL;
    p5 = hl_pool_alloc(auto_pool,sizeof(char)*1024);
    assert(p5);
    strncpy(p5,"Welcome to Dongjing",strlen("Welcome to Dongjing")+1);
    printf("p5:%s\n",p5);
    printf("using pool current_size:%d,total_size:%D\n",(int)auto_pool->using_pool->current_size,(int)auto_pool->using_pool->total_size);
    char * p6=NULL;
    p6 = hl_pool_alloc(auto_pool,sizeof(char)*1024);
    assert(p6);
    strncpy(p6,"Welcome to Tianjin",strlen("Welcome to Tianjin")+1);
    printf("p4:%s\n",p6);
    printf("using pool current_size:%d,total_size:%D\n",(int)auto_pool->using_pool->current_size,(int)auto_pool->using_pool->total_size);
    
    
    printf("p2  string:%s\n",p2);
    printf("p3  string:%s\n",p3);
    printf("p4  string:%s\n",p4);
     printf("p5  string:%s\n",p5);
     printf("p6  string:%s\n",p6);
    printf("errors of pool:%d\n",auto_pool->error_times);
    printf("errors of pool:%d\n",auto_pool->using_pool->error_times);
    printf("errors of pool:%d\n",auto_pool->using_pool->next->error_times);
}
void test_link(){
    hl_new_link(link);
    link->head = link;
    link->tail = link;
    if(link==NULL){
        DEBUG_MSG("got an error:alloc failed");
        exit(0);
    }
    link->data = (void *) hl_new_string("I am root");
    hl_new_link(new_item);
    new_item->data = (void * ) hl_new_string("Hello world!");
    hl_link_append(link,new_item);
    
    hl_new_link(new_item2);
    new_item2->data= (void*)hl_new_string("Yes it's cool");
    hl_link_append(new_item,new_item2);
    
    hl_new_link(new_item3);
    new_item3->data= (void*)hl_new_string("Yes it's not cool!");
    hl_link_append(new_item2,new_item3);
    
    
    hl_link * current = NULL;
    hl_string * str=NULL;
    hl_link_iter(link, current){
        printf("new row!");
//        if(current)
        str = (hl_string *)current->data;
        if(str!=NULL){
            printf("got data:%s\n",str->data);
        }
    }
    hl_link_iter_r(link, current){
        printf("new row!");
        //        if(current)
        str = (hl_string *)current->data;
        if(str!=NULL){
            printf("got data:%s\n",str->data);
        }
    }
    
    
}

void test_string(){
    hl_string * p;
    p = hl_new_string("Hello world");
    printf("string is:%s\n",p->data);
    hl_list * list;
    char * tmp_str;
    list = hl_new_list(16);
    int i=0;
    int result=0;
    hl_string * tmp_hl_string=NULL;
    for(i=0;i<20;i++){
        HL_CALLOC(tmp_str, char, 20);
        sprintf(tmp_str,"temp_str_{%d}",i);
        tmp_hl_string = hl_new_string(tmp_str);
        result = hl_list_append(list,tmp_hl_string);
        printf("list_append result:%d\n",result);
    }
    hl_string * return_data = NULL;
    hl_list_at(list,5,return_data);
    if(return_data!=NULL) {
        printf("got item at index 5:%s\n",return_data->data);
    }
    hl_string_free(p);
    hl_string_free(p);
    hl_string * item=NULL;
    while( TRUE){
        item = (hl_string *) hl_list_pop(list);
        if(item!=NULL){
            printf("got item:[%s]\n",item->data);
        }else{
            printf("reach the end\n");
            break;
        }
    }
    
}

