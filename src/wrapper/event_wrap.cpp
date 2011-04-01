//
//  libavg - Media Playback Engine. 
//  Copyright (C) 2003-2008 Ulrich von Zadow
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//  Current versions can be found at www.libavg.de
//

#include "WrapHelper.h"

#include "../player/KeyEvent.h"
#include "../player/MouseEvent.h"
#include "../player/TouchEvent.h"
#include "../player/CustomEvent.h"
#include "../player/CustomCursorEvent.h"
#include "../player/VisibleNode.h"
#include "../player/TrackerEventSource.h"

#include <string>

using namespace boost::python;
using namespace avg;
using namespace std;


class IEventSourceWrapper : public IEventSource, public wrapper<IEventSource> {
    public:
        IEventSourceWrapper(const std::string& name)
            : IEventSource(name)
        {
        }

        IEventSourceWrapper(const IEventSource& eventSource)
            : IEventSource(eventSource)
        {
        }

        virtual void start() {
            if (override startMethod = this->get_override("start")) {
                startMethod();
            }
            IEventSource::start();
        }

        void default_start() {
            return this->IEventSource::start();
        }

        virtual std::vector<EventPtr> pollEvents() {
            return this->get_override("pollEvents")();
        }

};


void export_event()
{
    boost::python::to_python_converter<vector<TouchEventPtr>, 
        to_tuple<vector<TouchEventPtr> > >();

    boost::python::to_python_converter<ContourSeq, to_list<ContourSeq> >();    
   
    from_python_sequence<ContourSeq, variable_capacity_policy>();
    from_python_sequence<vector<EventPtr>, variable_capacity_policy>();


    class_<Event, boost::noncopyable>("Event", no_init)
        .add_property("type", &Event::getType)
        .add_property("when", &Event::getWhen)
        .add_property("eventsource",
                      make_function(&Event::getEventSource,
                                    return_value_policy<reference_existing_object>()))
        .add_property("eventsourcename",
                      make_function(&Event::getEventSourceName,
                                    return_value_policy<copy_const_reference>()))
    ;

    class_<CustomEvent, bases<Event> >("CustomEvent", init<Event::Type, Event::Source, optional<int> >())
    ;

    class_<CustomCursorEvent, bases<Event> >("CustomCursorEvent",
                                             init<int, Event::Type, const IntPoint&, Event::Source>())
        .add_property("source", &CustomCursorEvent::getSource)
        .add_property("pos", &CustomCursorEvent::getPos)
        .add_property("x", &CustomCursorEvent::getXPosition)
        .add_property("y", &CustomCursorEvent::getYPosition)
        .add_property("cursorid", &CustomCursorEvent::getCursorID)
        .add_property("node", &CustomCursorEvent::getElement)
    ;

    enum_<Event::Type>("Type")
        .value("KEYUP", Event::KEYUP)
        .value("KEYDOWN", Event::KEYDOWN)
        .value("CURSORMOTION", Event::CURSORMOTION)
        .value("CURSORUP", Event::CURSORUP)
        .value("CURSORDOWN", Event::CURSORDOWN)
        .value("CURSOROVER", Event::CURSOROVER)
        .value("CURSOROUT", Event::CURSOROUT)
        .value("CUSTOMEVENT", Event::CUSTOMEVENT)
        .value("RESIZE", Event::RESIZE)
        .value("QUIT", Event::QUIT)
        .export_values()
    ;

    enum_<CursorEvent::Source>("Source")
        .value("MOUSE", CursorEvent::MOUSE)
        .value("TOUCH", CursorEvent::TOUCH)
        .value("TRACK", CursorEvent::TRACK)
        .value("CUSTOM", Event::CUSTOM)
        .value("NONE", Event::NONE)
        .export_values()
    ;

    enum_<int>("KeyModifier")
        .value("KEYMOD_NONE", key::KEYMOD_NONE)
        .value("KEYMOD_LSHIFT", key::KEYMOD_LSHIFT)
        .value("KEYMOD_RSHIFT", key::KEYMOD_RSHIFT)
        .value("KEYMOD_LCTRL", key::KEYMOD_LCTRL)
        .value("KEYMOD_RCTRL", key::KEYMOD_RCTRL)
        .value("KEYMOD_LALT", key::KEYMOD_LALT)
        .value("KEYMOD_RALT", key::KEYMOD_RALT)
        .value("KEYMOD_LMETA", key::KEYMOD_LMETA)
        .value("KEYMOD_RMETA", key::KEYMOD_RMETA)
        .value("KEYMOD_NUM", key::KEYMOD_NUM)
        .value("KEYMOD_CAPS", key::KEYMOD_CAPS)
        .value("KEYMOD_MODE", key::KEYMOD_MODE)
        .value("KEYMOD_RESERVED", key::KEYMOD_RESERVED)
        .value("KEYMOD_CTRL", key::KEYMOD_CTRL)
        .value("KEYMOD_SHIFT", key::KEYMOD_SHIFT)
        .value("KEYMOD_ALT", key::KEYMOD_ALT)
        .value("KEYMOD_META", key::KEYMOD_META)
        .export_values()
    ;

    class_<KeyEvent, bases<Event> >("KeyEvent", no_init)
        .add_property("scancode", &KeyEvent::getScanCode)
        .add_property("keycode", &KeyEvent::getKeyCode)
        .add_property("keystring", make_function(&KeyEvent::getKeyString, 
                return_value_policy<copy_const_reference>()))
        .add_property("unicode", &KeyEvent::getUnicode)
        .add_property("modifiers", &KeyEvent::getModifiers)
    ;    
    
    class_<MouseEvent, bases<Event> >("MouseEvent", no_init)
        .add_property("source", &MouseEvent::getSource)
        .add_property("leftbuttonstate", &MouseEvent::getLeftButtonState)
        .add_property("middlebuttonstate", &MouseEvent::getMiddleButtonState)
        .add_property("rightbuttonstate", &MouseEvent::getRightButtonState)
        .add_property("pos", &MouseEvent::getPos)
        .add_property("x", &MouseEvent::getXPosition)
        .add_property("y", &MouseEvent::getYPosition)
        .add_property("cursorid", &MouseEvent::getCursorID)
        .add_property("button", &MouseEvent::getButton)
        .add_property("node", &MouseEvent::getElement)
        .add_property("speed", make_function(&MouseEvent::getSpeed,
                return_value_policy<copy_const_reference>()))
        .add_property("lastdownpos", &CursorEvent::getLastDownPos)
        ;

    class_<TouchEvent, bases<Event> >("TouchEvent", no_init)
        .add_property("source", &TouchEvent::getSource)
        .add_property("area", &TouchEvent::getArea)
        .add_property("orientation", &TouchEvent::getOrientation)
        .add_property("eccentricity", &TouchEvent::getEccentricity)
        .add_property("pos", &TouchEvent::getPos)
        .add_property("x", &TouchEvent::getXPosition)
        .add_property("y", &TouchEvent::getYPosition)
        .add_property("cursorid", &TouchEvent::getCursorID)
        .add_property("node", &TouchEvent::getElement)
        .add_property("center", make_function(&TouchEvent::getCenter,
                return_value_policy<copy_const_reference>()))
        .add_property("majoraxis", make_function(&TouchEvent::getMajorAxis,
                return_value_policy<copy_const_reference>()))
        .add_property("minoraxis", make_function(&TouchEvent::getMinorAxis,
                return_value_policy<copy_const_reference>()))
        .add_property("speed", make_function(&TouchEvent::getSpeed,
                return_value_policy<copy_const_reference>()))
        .add_property("handorientation", &TouchEvent::getHandOrientation)
        .def("getRelatedEvents", &TouchEvent::getRelatedEvents)
        .def("getContour", &TouchEvent::getContour)
        .add_property("lastdownpos", &CursorEvent::getLastDownPos)
        ;
   
    enum_<TrackerImageID>("TrackerImageID")
        .value("IMG_CAMERA", TRACKER_IMG_CAMERA)
        .value("IMG_DISTORTED", TRACKER_IMG_DISTORTED)
        .value("IMG_NOHISTORY", TRACKER_IMG_NOHISTORY)
        .value("IMG_HISTOGRAM", TRACKER_IMG_HISTOGRAM)
        .value("IMG_FINGERS", TRACKER_IMG_FINGERS)
        .value("IMG_HIGHPASS", TRACKER_IMG_HIGHPASS)
        .export_values()
    ;

    class_<IEventSourceWrapper, auto_ptr<IEventSourceWrapper> >("EventSource", init<const std::string&>())
        .def("start", &IEventSource::start, &IEventSourceWrapper::default_start)
        .def("pollEvents", pure_virtual(&IEventSource::pollEvents))
        .add_property("name",
                      make_function(&IEventSource::getName,
                                    return_value_policy<copy_const_reference>()))
    ;

    class_<TrackerEventSource, boost::noncopyable>("Tracker", no_init)
        .def("getImage", &TrackerEventSource::getImage,
            return_value_policy<manage_new_object>())
        .def("getDisplayROIPos", &TrackerEventSource::getDisplayROIPos)
        .def("getDisplayROISize", &TrackerEventSource::getDisplayROISize)
        .def("saveConfig", &TrackerEventSource::saveConfig)
        .def("resetHistory", &TrackerEventSource::resetHistory)
        .def("setDebugImages", &TrackerEventSource::setDebugImages)
        .def("startCalibration", &TrackerEventSource::startCalibration,
            return_value_policy<reference_existing_object>())
        .def("endCalibration", &TrackerEventSource::endCalibration)
        .def("abortCalibration", &TrackerEventSource::abortCalibration)
        .def("setParam", &TrackerEventSource::setParam)
        .def("getParam", &TrackerEventSource::getParam)
        ;

    class_<TrackerCalibrator, boost::noncopyable>("TrackerCalibrator", no_init)
        .def("nextPoint", &TrackerCalibrator::nextPoint)
        .def("getDisplayPoint", &TrackerCalibrator::getDisplayPoint)
        .def("setCamPoint", &TrackerCalibrator::setCamPoint)
        ;
}
