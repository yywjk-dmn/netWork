#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <json-c/json_object.h>

#define BUFFER_SIZE 3

int main()
{
    /* 新建json对象 */
    struct json_object * jsonObj = json_object_new_object();
    if (jsonObj == NULL)
    {
        puerror("new json object error");
        exit(-1);

    }

    struct json_object * wayvalue = json_object_new_int64(32);
    if (wayvalue == NULL)
    {
        perror("new json object error");
        json_object_put(jsonObj);
        exit(-1);
    }

    /* 将key挂起 */
    int ret = json_object_object_add(jsonObj, "way", wayvalue);
    if (ret < 0)
    {
        perror("new json object error");
        exit(-1);
    }


    struct json_object * namevalue = json_object_new_string("zhangsan");
    json_object_object_add(jsonObj, "name", namevalue);

    /* 创建json对象的数组 */
    struct json_object * array = json_object_new_array();
    json_object_array_add(array, json_object_new_string("jingdong"));
    json_object_array_add(array, json_object_new_string("pingduoduo"));
    json_object_array_add(array, json_object_new_string("taobao"));

    /* 创建json的关键字的关键字 */
    struct json_object * newObject = json_object_new_object();
    json_object_object_add(newObject, "site1", json_object_new_string("www.runoob.com"));


    /* 将json对象转换成字符串 */
    const char * str = json_object_to_json_string(jsonObj);
    printf("str:%s\n", str);

    /* 需要将以上的内容传到服务器 下面就要写服务器的代码 */

    /* 将字符串转换成json对象 */
    struct json_object * newJsonObj = json_object_new_string(str);
    
    struct json_object * wayVal = json_object_object_get(newJsonObj, "way");
    if (wayVal == NULL)
    {
        perror("get wayVal error\n");
        exit(-1);
    }
    printf("value:%d\n", json_object_get_int64(wayVal));

    struct json_object * nameVal = json_object_object_get(newJsonObj, "name");
    if (nameVal == NULL)
    {
        perror("get nameVal error");
        exit(-1);
    }
    printf("nameVal:%s\n", json_object_get_string(nameVal));

    struct json_object * array = json_object_object_get(newJsonObj, "shopping");
    if (array == NULL)
    {
        perror("get array error");
        exit(-1);
    }
#if 0
    /* 转义符号 */
    char * ptr = "hello\\world";
    int len = strlen(ptr);
    printf("len:%d\n", len);
    printf("ptr : %s\n", ptr);
#endif
    /* 释放内存 */
    json_object_put(jsonObj);
    return 0;


}