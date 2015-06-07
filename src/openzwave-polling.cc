/*
* Copyright (c) 2013 Jonathan Perkin <jonathan@perkin.org.uk>
* Copyright (c) 2015 Elias Karakoulakis <elias.karakoulakis@gmail.com>
* 
* Permission to use, copy, modify, and distribute this software for any
* purpose with or without fee is hereby granted, provided that the above
* copyright notice and this permission notice appear in all copies.
*
* THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
* WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
* ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
* WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
* ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "openzwave.hpp"
using namespace v8;
using namespace node;

namespace OZW {

 	// Get the time period between polls of a node's state
	// ===================================================================
	NAN_METHOD(GetPollInterval)
	// ===================================================================
	{
		NanScope();
		uint32 i = OpenZWave::Manager::Get()->GetPollInterval();
		NanReturnValue(NanNew<Integer>(i));
	}
	
	/* Set the time period between polls of a node's state. Due to patent 
	 * concerns, some devices do not report state changes automatically to 
	 * the controller. These devices need to have their state polled at 
	 * regular intervals. The length of the interval is the same for all 
	 * devices. To even out the Z-Wave network traffic generated by 
	 * polling, OpenZWave divides the polling interval by the number of 
	 * devices that have polling enabled, and polls each in turn. 
	 * It is recommended that if possible, the interval should not be set 
	 * shorter than the number of polled devices in seconds (so that the 
	 * network does not have to cope with more than one poll per second). 
	 */ 
	// ===================================================================
	NAN_METHOD(SetPollInterval)
	// ===================================================================
	{
		NanScope();
		uint8_t intervalMillisecs = args[0]->ToNumber()->Value();
		OpenZWave::Manager::Get()->SetPollInterval (intervalMillisecs, false);
		NanReturnUndefined();
	}
	
	
	/*
	* Enable/Disable polling on a COMMAND_CLASS basis.
	*/
	// ===================================================================
	NAN_METHOD(EnablePoll)
	// ===================================================================
	{
		NanScope();

		uint8_t nodeid = args[0]->ToNumber()->Value();
		uint8_t comclass = args[1]->ToNumber()->Value();
		NodeInfo *node;
		std::list<OpenZWave::ValueID>::iterator vit;

		if ((node = get_node_info(nodeid))) {
			for (vit = node->values.begin(); vit != node->values.end(); ++vit) {
				if ((*vit).GetCommandClassId() == comclass) {
					OpenZWave::Manager::Get()->EnablePoll((*vit), 1);
					break;
				}
			}
		}

		NanReturnUndefined();
	}

	// ===================================================================
	NAN_METHOD(DisablePoll)
	// ===================================================================
	{
		NanScope();

		uint8_t nodeid = args[0]->ToNumber()->Value();
		uint8_t comclass = args[1]->ToNumber()->Value();
		NodeInfo *node;
		std::list<OpenZWave::ValueID>::iterator vit;

		if ((node = get_node_info(nodeid))) {
			for (vit = node->values.begin(); vit != node->values.end(); ++vit) {
				if ((*vit).GetCommandClassId() == comclass) {
					OpenZWave::Manager::Get()->DisablePoll((*vit));
					break;
				}
			}
		}

		NanReturnUndefined();
	}
	
	// Determine the polling of a device's state.
	// ===================================================================
	NAN_METHOD(IsPolled)
	// ===================================================================
	{
		NanScope();
		
		uint8_t nodeid = args[0]->ToNumber()->Value();
		uint8_t comclass = args[1]->ToNumber()->Value();
		uint8_t instance = args[2]->ToNumber()->Value();
		uint8_t index = args[3]->ToNumber()->Value();

		NodeInfo *node;
		std::list<OpenZWave::ValueID>::iterator vit;

		if ((node = get_node_info(nodeid))) {
			for (vit = node->values.begin(); vit != node->values.end(); ++vit) {
				if (((*vit).GetCommandClassId() == comclass) && ((*vit).GetInstance() == instance) && ((*vit).GetIndex() == index)) {
					bool b = OpenZWave::Manager::Get()->isPolled(*vit);
					NanReturnValue(NanNew<Boolean>(b));
				}
			}
		}
		
		NanReturnUndefined();
	}
	
	// ===================================================================
	NAN_METHOD(SetPollIntensity)
	// ===================================================================
	{
		NanScope();
		
		uint8_t nodeid = args[0]->ToNumber()->Value();
		uint8_t comclass = args[1]->ToNumber()->Value();
		uint8_t instance = args[2]->ToNumber()->Value();
		uint8_t index = args[3]->ToNumber()->Value();
		uint8_t intensity = args[4]->ToNumber()->Value();
		
		NodeInfo *node;
		std::list<OpenZWave::ValueID>::iterator vit;

		if ((node = get_node_info(nodeid))) {
			for (vit = node->values.begin(); vit != node->values.end(); ++vit) {
				if (((*vit).GetCommandClassId() == comclass) && ((*vit).GetInstance() == instance) && ((*vit).GetIndex() == index)) {
					
					OpenZWave::Manager::Get()->SetPollIntensity (*vit, intensity);
					NanReturnUndefined();
				}
			}
		}
		NanReturnUndefined();
	}

 	// Get the polling intensity of a device's state. 
	// ===================================================================
	NAN_METHOD(GetPollIntensity)
	// ===================================================================
	{
		NanScope();
		
		uint8_t nodeid = args[0]->ToNumber()->Value();
		uint8_t comclass = args[1]->ToNumber()->Value();
		uint8_t instance = args[2]->ToNumber()->Value();
		uint8_t index = args[3]->ToNumber()->Value();

		NodeInfo *node;
		std::list<OpenZWave::ValueID>::iterator vit;

		if ((node = get_node_info(nodeid))) {
			for (vit = node->values.begin(); vit != node->values.end(); ++vit) {
				if (((*vit).GetCommandClassId() == comclass) && ((*vit).GetInstance() == instance) && ((*vit).GetIndex() == index)) {
					uint8 i = OpenZWave::Manager::Get()->GetPollIntensity(*vit);
					NanReturnValue(NanNew<Integer>(i));
				}
			}
		}
		
		NanReturnUndefined();
	}
}
