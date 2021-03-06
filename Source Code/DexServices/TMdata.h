
// Telemetry types for GRIP and GRASP, based on example programs from Qinetiq.

#pragma once

#include "../Trackers/Trackers.h"
#include "../Trackers/PoseTrackers.h"
#include "../VectorsMixin/VectorsMixin.h"
#include "../Grip/GripPackets.h"

typedef unsigned short u16;
typedef signed short s16;
typedef unsigned u32;
typedef signed s32;
typedef unsigned char u8;
typedef float f32;
typedef char sz;

class GraspPacket : public PsyPhy::VectorsMixin
{
  public:
    virtual u32 serialize(u8 *buffer) const = 0;
    virtual u16 getPktType() const = 0;
  protected:
    u32 addHeader(u8 *buffer) const ;

};

class HK_packet : public GraspPacket
{
    public:
        virtual u32 serialize(u8 *buffer) const;
        virtual u16 getPktType() const;

        u16 current_user;
        u16 current_protocol;
        u16 current_task;
        u16 current_step;
        u16 scriptengine_status;
        u16 motiontracker_status;
};

class RT_packet : public GraspPacket
{
    public:
        virtual u32 serialize(u8 *buffer) const;
        virtual u16 getPktType() const;

        u32 rtdata_acq_id;
        u32 rtdata_pkt_counter;

		struct {
			float	fillTime;
			u32		globalCount;
			PsyPhy::TrackerPose	hmd;
			PsyPhy::TrackerPose	hand;
			PsyPhy::TrackerPose	chest;
			MarkerFrame markerFrame[2];
			float	clientTime;
			unsigned char clientData[GRASP_RT_CLIENT_BYTES];
		} Slice[GRASP_RT_SLICES_PER_PACKET];

		RT_packet( void ) : rtdata_acq_id(0x12341234) , rtdata_pkt_counter( 0 ) {}
			
};

class CameraTrigger_packet : public GraspPacket
{
    public:
        virtual u32 serialize(u8 *buffer) const;
        virtual u16 getPktType() const;

        sz tag[10];
};

 u32 extractTM(u16 &tm, u8 *buffer, u32 pos);
 u32 extractTM(s16 &tm, u8 *buffer, u32 pos);
 u32 extractTM(u32 &tm, u8 *buffer, u32 pos);
 u32 extractTM(s32 &tm, u8 *buffer, u32 pos);
 u32 extractTM(f32 &tm, u8 *buffer, u32 pos);
 u32 extractTM( u8 &tm, u8 *buffer, u32 pos);
 u32 extractTM( u8 *tm, int bytes, u8 *buffer, u32 pos);
 u32 extractTM(bool &tm, u8*buffer, u32 pos);
 u32 extractTM( PsyPhy::TrackerPose &pose, u8 *buffer, u32 pos );
 u32 extractTM( MarkerState &m, u8 *buffer, u32 pos );
 u32 extractTM( MarkerFrame &frame, u8 *buffer, u32 pos );

