#include <cmath>
#include "global_variables.h"
#include "EventList.h"
#include "RemoveVehicleEvent.h"
#include "AddVehicleEvent.h"
#include "Street.h"

RemoveVehicleEvent::RemoveVehicleEvent(const int &time, Street *street, EventList* events)
    : Event(time, street, events) {
}

RemoveVehicleEvent::~RemoveVehicleEvent() {
}

void RemoveVehicleEvent::makeItHappen() {

	// If the street is not empty, then remove the first vehicle
	if (!m_street->isEmpty()) {

		// Get pointer to first vehicle
		Vehicle* vehicle = m_street->peek();
	
		if (m_street->isDrain()) {

			// Drain street removal
			m_street->removeVehicle();

			// This vehicle won't be used anymore
			delete(vehicle);

			// Log it
			logfile << "\nVehicle n. " << vehicle->getID()
				    << " permanently removed from " << m_street->getName();

		} else {

			// Test for greenlight. Only useful for vehicles other than the first.
			if (m_street->isGreenLight()) {
				
				// Test for time of arrival at the stoplight.
				// Only useful for the first vehicle
				if (vehicle->getRemTime() <= sim_clock) {

					// Get destination street
					int route = vehicle->getRoute();
					Street* dest_street = m_street->getDestinationStreet(route);

					// Get available space in the destination street
					int free_space = dest_street->getFreeSpace();

					if (vehicle->getLength() + 3 <= free_space) {
						// Remove vehicle from this street
						m_street->removeVehicle();
			
						// Log it
						logfile << "\nVehicle n. " << vehicle->getID()
								<< " left " << m_street->getName()
								<< " towards " << dest_street->getName();

						// Compute how much time will it take for the vehicle to cross the
						// street. Also, this is the exact time needed for the second
						// vehicle on this street to be just under the traffic lights

						double speed_mps = m_street->getSpeed() / 3.6;
						int tmp_time = sim_clock + (int) ceil((vehicle->getLength() + 3) / speed_mps);

						// If this street isn't empty after the removal, then schedule
						// a new removal event.
						if (!m_street->isEmpty()) {
						
							// Check the next vehicle "distance" from the recently removed one
							if (m_street->peek()->getRemTime() <= tmp_time) {
								// Next vehicle was right behind the one that left
								m_events->sorted_insert(	new RemoveVehicleEvent(
									tmp_time, m_street, m_events));
							} else {
								// Either next vehicle is at some distance from the one that left							
								m_events->sorted_insert(	new RemoveVehicleEvent(
										m_street->peek()->getRemTime(), m_street, m_events));
							}
						}

						// Create an event to add the vehicle to destination street
						// We're not taking into account the time it takes for the vehicle
						// to cross the intersection. Teleportation ftw, folks!
						// Also, if the destination street is a drain, a corresponding remove event
						// will be created at the time of insertion (at AddVehicleEvent#makeItHappen).
						m_events->sorted_insert(new AddVehicleEvent(tmp_time, dest_street, m_events, vehicle));
					}
				} else {
					// First vehicle wasn't under the stoplight yet and couldn't be removed
					m_events->sorted_insert(	new RemoveVehicleEvent(
							m_street->peek()->getRemTime(), m_street, m_events));
				}
			}  // greenlight
		}
	}
}