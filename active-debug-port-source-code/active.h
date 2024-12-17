#ifndef ACTIVE_DEBUG_H
#define ACTIVE_DEBUG_H
   
void ACTIVEInit( void );                             // Initialize any hardware needed by the ACTIVE Interface
void ACTIVEValue( int channel, int value );          // Output a Value for this channel
void ACTIVEText( int channel, char *string );        // Output Text for this channel
void ACTIVEprintf( int channel, char *format, ... ); // printf-like function with variable argument list

// Define helpful macros for sending debug command messages
#define ACTIVELabel(channel,string)  ACTIVEText( (channel) , "?cmd=label&label=" string )
#define ACTIVEBeep()  ACTIVEText( 0 , "?cmd=beep" )
#define ACTIVEStop()  ACTIVEText( 0 , "?cmd=stop" )
#define ACTIVERestart()  ACTIVEText( 0 , "?cmd=restart" )

#endif
