#include <stdio.h>
#include <string.h>
#include <json-c/json.h>
#include <json-c/json_object.h>


int main()
{
    /* 新建json对象 */
    struct json_object * jsonObj = json_object_new_object();

    struct json_object * value = json_object_new_int64(32);

    json_object_object_add(jsonObj, "age", value);

    /* 将json对象转换成字符串 */
    const char * str = json_object_to_json_string(jsonObj);

    printf("str:%s\n", str);

    /* 将字符串转换成json对象 */
    struct json_object * ageObj = json_object_new_string(str);
    
    struct json_object * ageObj = json_tokener_parse(str);

    //struct json_object * keyValue = json_object_object_get(ageObj, key);

    json_object_object_get(ageObj, "age");

    const char * val = json_object_get_string(value);
    printf("value:%s\n", val);

    
    


#if 0
    /* 转义符号 */
    char * ptr = "hello\\world";
    int len = strlen(ptr);
    printf("len:%d\n", len);
    printf("ptr : %s\n", ptr);
#endif
    return 0;


}