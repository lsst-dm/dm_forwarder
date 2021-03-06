/*
 * This file is part of dm_forwarder
 *
 * Developed for the LSST Data Management System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef READOUT_PATTERN_H
#define READOUT_PATTERN_H

#include <yaml-cpp/yaml.h>
#include <daq/Sensor.hh>

class ReadoutPattern {
    public:
        ReadoutPattern(const YAML::Node& n);
        std::vector<std::string> data_segment_name(DAQ::Sensor::Type& sensor);
        std::vector<int> data_segment(DAQ::Sensor::Type& sensor);
        std::vector<int> sensor_order(DAQ::Sensor::Type& sensor);
        int get_xor(DAQ::Sensor::Type& sensor);

        static DAQ::Sensor::Type sensor(const std::string& location);

    private:
        YAML::Node _root;
};

#endif
