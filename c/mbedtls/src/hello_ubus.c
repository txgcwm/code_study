#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

#include <libubus.h>

#include <libubox/uloop.h>
#include <libubox/list.h>
#include <libubox/blobmsg_json.h>
#include <json-c/json.h>

struct ubus_context *ctx;
struct blob_buf b;

enum {
    HELLO_ID,
    HELLO_MSG,
    HELLO_ARRAY,
    HELLO_MAX,
};

static const struct blobmsg_policy hello_policy[HELLO_MAX] = {
    [HELLO_ID] = { .name = "id", .type = BLOBMSG_TYPE_INT32 },
    [HELLO_MSG] = { .name = "msg", .type = BLOBMSG_TYPE_STRING },
    [HELLO_ARRAY] = { .name = "array", .type = BLOBMSG_TYPE_ARRAY },
};

// ubus call test_ubus helloworld '{"id":1,"msg":"test_msg_hello_world"}' 
static int test_hello(struct ubus_context *ctx, struct ubus_object *obj, struct ubus_request_data *req,
    const char *method, struct blob_attr *msg)
{
    struct blob_attr *tb[HELLO_MAX];
    int tmp_id;
    char *tmp_msg = NULL;
    char tmp_array[128];
    int len;
    struct blob_attr *attr;
    void *arr;

    blobmsg_parse(hello_policy, HELLO_MAX, tb, blob_data(msg), blob_len(msg));

    blob_buf_init(&b, 0);
    if(tb[HELLO_ID]) {
        tmp_id = blobmsg_get_u32(tb[HELLO_ID]);
        blobmsg_add_u32(&b, "id", tmp_id); 
    }

    if(tb[HELLO_MSG]) {
        tmp_msg = blobmsg_get_string(tb[HELLO_MSG]);
        blobmsg_add_string(&b, "msg", tmp_msg);
    }

    if(tb[HELLO_ARRAY] && blobmsg_type(tb[HELLO_ARRAY]) == BLOBMSG_TYPE_ARRAY) {
        arr = blobmsg_open_array(&b, "shuzu");

        len = blobmsg_data_len(tb[HELLO_ARRAY]);
        __blob_for_each_attr(attr, blobmsg_data(tb[HELLO_ARRAY]), len) {
            if (blobmsg_type(attr) == BLOBMSG_TYPE_STRING) {
                char *tmp = blobmsg_get_string(attr);
                blobmsg_add_blob(&b, attr);
                printf("array1=%s\n", tmp);
            }
        }
        blobmsg_close_array(&b, arr);
    }

    printf("tmp_id=%d, tmp_msg=%s, tmp_array=%s\n", tmp_id, tmp_msg, tmp_array);

    ubus_send_reply(ctx, req, b.head);

    return 0;
} 

static const struct ubus_method test_methods[] = {
    UBUS_METHOD("helloworld", test_hello, hello_policy),
};

static struct ubus_object_type test_object_type = 
    UBUS_OBJECT_TYPE("test_ubus", test_methods);

static struct ubus_object test_object = {
    .name = "test_ubus",
    .type = &test_object_type,
    .methods = test_methods,
    .n_methods = ARRAY_SIZE(test_methods)
};

int ubus_doing()
{
    int ret;

    ctx = ubus_connect(NULL);
    if (!ctx) {
        fprintf(stderr, "Failed to connect to ubus\n");
        return -1;
    }
    ubus_add_uloop(ctx);

    ret = ubus_add_object(ctx, &test_object);
    if (ret) {
        fprintf(stderr, "Failed to add object: %s\n", ubus_strerror(ret));
    }

    return 0;
}

int main(int argc, char** argv)
{
    int ret;

    uloop_init();
    ubus_doing();
    uloop_run();

    ubus_free(ctx);
    uloop_done();

    return 0;
}

