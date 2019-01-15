# Command API description
This page describes the API corresponding to the [commands](https://bitbucket.org/sourceengineers/iot-scope-doc/src/master/Protocol.md) defined in the protocol.
# cf_running
The channels can either be stopped Scope_setChannelStopped or started Scope_setChannelRunning.
```c
/* Sets the channel with the given index to running */
void Scope_setChannelRunning(ScopeHandle self, uint32_t channelId);
/* Sets the channel with the given index to stopped */
void Scope_setChannelStopped(ScopeHandle self, uint32_t channelId);
```
The channelId must not exceed the maximum amount of channels.
# ev_poll
```c
/* Polls data from all channels */
void Scope_poll(ScopeHandle self, uint32_t timeStamp);
```
If the scope is set to TIMESTAMP_AUTOMATIC, the Scope_poll function ignores the timestamp value supplied.
# ev_trans
```c
/* Packs all the necessary data into a package ready to be sent */
void Scope_transmitData(ScopeHandle self);
```
This function works only if the transmitCallback is properly implemented. This is the same as for the normal ev_trans command.
# ev_announce
```c
/* Sends all configured watch addresses to the host */
void Scope_announceAddresses(ScopeHandle self);
```
# cf_addr
```c
/* Configures the channel with the given id, with the wanted address */
/* If the id exceeds the maximum amount of channels, the function will return without doing anything */
void Scope_configureChannel(ScopeHandle self, const size_t channelId, void* pollAddress, DATA_TYPES type);
```
# cf_tgr
```c
/* Configurates the trigger with the gives values
 * level: can be any float value
 * edge: This can be either TRIGGER_EDGE_NEGATIVE or TRIGGER_EDGE_POSITIVE
 * mode: Chooses the trigger mode. Can be either: TRIGGER_CONTINUOUS, TRIGGER_NORMAL or TRIGGER_ONESHOT
 * channelId: The id which should be watched by the trigger
 *
 * If the id exceeds the maximum amount of channels, the function will return without doing anything
 * */
void Scope_configureTrigger(ScopeHandle self, const float level, int edge, TRIGGER_MODE mode, uint32_t channelId);
```
# cf_t_inc
```c
/* Sets the timestamp increment
 * If the timestamp increment mode is set to manual, this will not have any effect */
void Scope_configureTimestampIncrement(ScopeHandle self, uint32_t timstampIncrement);
```
The Scope_configureTimestampIncrement does only have a affect on the scope timestamping mode is set to TIMESTAMP_AUTOMATIC
# ev_clear
```c
/* Clears the timestamp and all the channels */
void Scope_clear(ScopeHandle self);
```
