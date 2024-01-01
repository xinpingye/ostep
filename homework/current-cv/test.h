#ifndef __test_h__
#define __test_h__

#define test0 do_test(id, 1);

void do_test(int id, int tmp)
{
    printf("id + tmp = %d", (id + tmp));
    return;
}

#endif