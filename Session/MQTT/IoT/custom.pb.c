/* Automatically generated nanopb constant definitions */
/* Generated by nanopb-0.3.6 at Tue Sep 27 10:29:54 2016. */

#include "custom.pb.h"

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif



const pb_field_t Custom_fields[1] = {
    PB_LAST_FIELD
};

const pb_field_t Custom__Header_fields[5] = {
    PB_FIELD(  1, UENUM   , REQUIRED, STATIC  , FIRST, Custom__Header, command, command, 0),
    PB_FIELD(  2, STRING  , OPTIONAL, STATIC  , OTHER, Custom__Header, originator, command, 0),
    PB_FIELD(  3, STRING  , OPTIONAL, CALLBACK, OTHER, Custom__Header, nestedPath, originator, 0),
    PB_FIELD(  4, STRING  , OPTIONAL, CALLBACK, OTHER, Custom__Header, nestedSpec, nestedPath, 0),
    PB_LAST_FIELD
};

const pb_field_t Custom_serialPrintln_fields[2] = {
    PB_FIELD(  1, STRING  , REQUIRED, STATIC  , FIRST, Custom_serialPrintln, message, message, 0),
    PB_LAST_FIELD
};

const pb_field_t Custom_ping_fields[1] = {
    PB_LAST_FIELD
};

const pb_field_t Custom_testEvents_fields[1] = {
    PB_LAST_FIELD
};


/* @@protoc_insertion_point(eof) */
