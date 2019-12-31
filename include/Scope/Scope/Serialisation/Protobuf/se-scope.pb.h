/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.0-dev */

#ifndef PB_PB_SE_SCOPE_PB_H_INCLUDED
#define PB_PB_SE_SCOPE_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Enum definitions */
typedef enum _PB_Var_Type {
    PB_Var_Type_SE_UINT8 = 0,
    PB_Var_Type_SE_UINT16 = 1,
    PB_Var_Type_SE_UINT32 = 2,
    PB_Var_Type_SE_FLOAT = 3
} PB_Var_Type;

typedef enum _PB_Trigger_Mode {
    PB_Trigger_Mode_NORMAL = 0,
    PB_Trigger_Mode_CONTINUOUS = 1,
    PB_Trigger_Mode_ONE_SHOT = 2
} PB_Trigger_Mode;

/* Struct definitions */
typedef struct _PB_CF_Address {
    pb_callback_t addresses;
} PB_CF_Address;

typedef struct _PB_CF_Running {
    pb_callback_t running;
} PB_CF_Running;

typedef struct _PB_AddressConfig {
    uint32_t cl_id;
    uint32_t address;
    PB_Var_Type type;
} PB_AddressConfig;

typedef struct _PB_CF_TInc {
    uint32_t t_inc;
} PB_CF_TInc;

typedef struct _PB_CF_Trigger {
    uint32_t cl_id;
    PB_Trigger_Mode mode;
    float level;
    bool edge;
} PB_CF_Trigger;

typedef struct _PB_EV_Poll {
    uint32_t timestamp;
} PB_EV_Poll;

typedef struct _PB_RunningConfig {
    uint32_t cl_id;
    bool new_state;
} PB_RunningConfig;

typedef struct _PB_SC_Channel {
    pb_callback_t data;
    uint32_t id;
} PB_SC_Channel;

typedef struct _PB_SC_Trigger {
    uint32_t cl_id;
    uint32_t cl_data_ind;
    PB_Trigger_Mode mode;
} PB_SC_Trigger;

typedef struct _PB_SC_Data {
    pb_callback_t channels;
    pb_callback_t timestamps;
    uint32_t t_inc;
    bool has_trigger;
    PB_SC_Trigger trigger;
} PB_SC_Data;


/* Helper constants for enums */
#define _PB_Var_Type_MIN PB_Var_Type_SE_UINT8
#define _PB_Var_Type_MAX PB_Var_Type_SE_FLOAT
#define _PB_Var_Type_ARRAYSIZE ((PB_Var_Type)(PB_Var_Type_SE_FLOAT+1))

#define _PB_Trigger_Mode_MIN PB_Trigger_Mode_NORMAL
#define _PB_Trigger_Mode_MAX PB_Trigger_Mode_ONE_SHOT
#define _PB_Trigger_Mode_ARRAYSIZE ((PB_Trigger_Mode)(PB_Trigger_Mode_ONE_SHOT+1))


/* Initializer values for message structs */
#define PB_SC_Trigger_init_default               {0, 0, _PB_Trigger_Mode_MIN}
#define PB_SC_Channel_init_default               {{{NULL}, NULL}, 0}
#define PB_SC_Data_init_default                  {{{NULL}, NULL}, {{NULL}, NULL}, 0, false, PB_SC_Trigger_init_default}
#define PB_AddressConfig_init_default            {0, 0, _PB_Var_Type_MIN}
#define PB_CF_Address_init_default               {{{NULL}, NULL}}
#define PB_RunningConfig_init_default            {0, 0}
#define PB_CF_Running_init_default               {{{NULL}, NULL}}
#define PB_CF_Trigger_init_default               {0, _PB_Trigger_Mode_MIN, 0, 0}
#define PB_CF_TInc_init_default                  {0}
#define PB_EV_Poll_init_default                  {0}
#define PB_SC_Trigger_init_zero                  {0, 0, _PB_Trigger_Mode_MIN}
#define PB_SC_Channel_init_zero                  {{{NULL}, NULL}, 0}
#define PB_SC_Data_init_zero                     {{{NULL}, NULL}, {{NULL}, NULL}, 0, false, PB_SC_Trigger_init_zero}
#define PB_AddressConfig_init_zero               {0, 0, _PB_Var_Type_MIN}
#define PB_CF_Address_init_zero                  {{{NULL}, NULL}}
#define PB_RunningConfig_init_zero               {0, 0}
#define PB_CF_Running_init_zero                  {{{NULL}, NULL}}
#define PB_CF_Trigger_init_zero                  {0, _PB_Trigger_Mode_MIN, 0, 0}
#define PB_CF_TInc_init_zero                     {0}
#define PB_EV_Poll_init_zero                     {0}

/* Field tags (for use in manual encoding/decoding) */
#define PB_CF_Address_addresses_tag              1
#define PB_CF_Running_running_tag                1
#define PB_AddressConfig_cl_id_tag               1
#define PB_AddressConfig_address_tag             2
#define PB_AddressConfig_type_tag                3
#define PB_CF_TInc_t_inc_tag                     1
#define PB_CF_Trigger_cl_id_tag                  1
#define PB_CF_Trigger_mode_tag                   2
#define PB_CF_Trigger_level_tag                  3
#define PB_CF_Trigger_edge_tag                   4
#define PB_EV_Poll_timestamp_tag                 1
#define PB_RunningConfig_cl_id_tag               1
#define PB_RunningConfig_new_state_tag           2
#define PB_SC_Channel_data_tag                   1
#define PB_SC_Channel_id_tag                     2
#define PB_SC_Trigger_cl_id_tag                  1
#define PB_SC_Trigger_cl_data_ind_tag            2
#define PB_SC_Trigger_mode_tag                   3
#define PB_SC_Data_channels_tag                  1
#define PB_SC_Data_timestamps_tag                2
#define PB_SC_Data_t_inc_tag                     3
#define PB_SC_Data_trigger_tag                   4

/* Struct field encoding specification for nanopb */
#define PB_SC_Trigger_FIELDLIST(X, a) \
X(a, STATIC, SINGULAR, UINT32, cl_id, 1) \
X(a, STATIC, SINGULAR, UINT32, cl_data_ind, 2) \
X(a, STATIC, SINGULAR, UENUM, mode, 3)
#define PB_SC_Trigger_CALLBACK NULL
#define PB_SC_Trigger_DEFAULT NULL

#define PB_SC_Channel_FIELDLIST(X, a) \
X(a, CALLBACK, REPEATED, FLOAT, data, 1) \
X(a, STATIC, SINGULAR, UINT32, id, 2)
#define PB_SC_Channel_CALLBACK pb_default_field_callback
#define PB_SC_Channel_DEFAULT NULL

#define PB_SC_Data_FIELDLIST(X, a) \
X(a, CALLBACK, REPEATED, MESSAGE, channels, 1) \
X(a, CALLBACK, REPEATED, FIXED32, timestamps, 2) \
X(a, STATIC, SINGULAR, UINT32, t_inc, 3) \
X(a, STATIC, OPTIONAL, MESSAGE, trigger, 4)
#define PB_SC_Data_CALLBACK pb_default_field_callback
#define PB_SC_Data_DEFAULT NULL
#define PB_SC_Data_channels_MSGTYPE PB_SC_Channel
#define PB_SC_Data_trigger_MSGTYPE PB_SC_Trigger

#define PB_AddressConfig_FIELDLIST(X, a) \
X(a, STATIC, SINGULAR, UINT32, cl_id, 1) \
X(a, STATIC, SINGULAR, UINT32, address, 2) \
X(a, STATIC, SINGULAR, UENUM, type, 3)
#define PB_AddressConfig_CALLBACK NULL
#define PB_AddressConfig_DEFAULT NULL

#define PB_CF_Address_FIELDLIST(X, a) \
X(a, CALLBACK, REPEATED, MESSAGE, addresses, 1)
#define PB_CF_Address_CALLBACK pb_default_field_callback
#define PB_CF_Address_DEFAULT NULL
#define PB_CF_Address_addresses_MSGTYPE PB_AddressConfig

#define PB_RunningConfig_FIELDLIST(X, a) \
X(a, STATIC, SINGULAR, UINT32, cl_id, 1) \
X(a, STATIC, SINGULAR, BOOL, new_state, 2)
#define PB_RunningConfig_CALLBACK NULL
#define PB_RunningConfig_DEFAULT NULL

#define PB_CF_Running_FIELDLIST(X, a) \
X(a, CALLBACK, REPEATED, MESSAGE, running, 1)
#define PB_CF_Running_CALLBACK pb_default_field_callback
#define PB_CF_Running_DEFAULT NULL
#define PB_CF_Running_running_MSGTYPE PB_RunningConfig

#define PB_CF_Trigger_FIELDLIST(X, a) \
X(a, STATIC, SINGULAR, UINT32, cl_id, 1) \
X(a, STATIC, SINGULAR, UENUM, mode, 2) \
X(a, STATIC, SINGULAR, FLOAT, level, 3) \
X(a, STATIC, SINGULAR, BOOL, edge, 4)
#define PB_CF_Trigger_CALLBACK NULL
#define PB_CF_Trigger_DEFAULT NULL

#define PB_CF_TInc_FIELDLIST(X, a) \
X(a, STATIC, SINGULAR, UINT32, t_inc, 1)
#define PB_CF_TInc_CALLBACK NULL
#define PB_CF_TInc_DEFAULT NULL

#define PB_EV_Poll_FIELDLIST(X, a) \
X(a, STATIC, SINGULAR, UINT32, timestamp, 1)
#define PB_EV_Poll_CALLBACK NULL
#define PB_EV_Poll_DEFAULT NULL

extern const pb_msgdesc_t PB_SC_Trigger_msg;
extern const pb_msgdesc_t PB_SC_Channel_msg;
extern const pb_msgdesc_t PB_SC_Data_msg;
extern const pb_msgdesc_t PB_AddressConfig_msg;
extern const pb_msgdesc_t PB_CF_Address_msg;
extern const pb_msgdesc_t PB_RunningConfig_msg;
extern const pb_msgdesc_t PB_CF_Running_msg;
extern const pb_msgdesc_t PB_CF_Trigger_msg;
extern const pb_msgdesc_t PB_CF_TInc_msg;
extern const pb_msgdesc_t PB_EV_Poll_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define PB_SC_Trigger_fields &PB_SC_Trigger_msg
#define PB_SC_Channel_fields &PB_SC_Channel_msg
#define PB_SC_Data_fields &PB_SC_Data_msg
#define PB_AddressConfig_fields &PB_AddressConfig_msg
#define PB_CF_Address_fields &PB_CF_Address_msg
#define PB_RunningConfig_fields &PB_RunningConfig_msg
#define PB_CF_Running_fields &PB_CF_Running_msg
#define PB_CF_Trigger_fields &PB_CF_Trigger_msg
#define PB_CF_TInc_fields &PB_CF_TInc_msg
#define PB_EV_Poll_fields &PB_EV_Poll_msg

/* Maximum encoded size of messages (where known) */
#define PB_SC_Trigger_size                       14
/* PB_SC_Channel_size depends on runtime parameters */
/* PB_SC_Data_size depends on runtime parameters */
#define PB_AddressConfig_size                    14
/* PB_CF_Address_size depends on runtime parameters */
#define PB_RunningConfig_size                    8
/* PB_CF_Running_size depends on runtime parameters */
#define PB_CF_Trigger_size                       15
#define PB_CF_TInc_size                          6
#define PB_EV_Poll_size                          6

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
