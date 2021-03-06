// *****************************************************************************
//
// Copyright (c) 2014, Southwest Research Institute® (SwRI®)
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Southwest Research Institute® (SwRI®) nor the
//       names of its contributors may be used to endorse or promote products
//       derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// *****************************************************************************

#ifndef TRANSFORM_UTIL_LOCAL_XY_UTIL_H_
#define TRANSFORM_UTIL_LOCAL_XY_UTIL_H_

#include <string>

#include <boost/shared_ptr.hpp>

#include <ros/ros.h>
#include <topic_tools/shape_shifter.h>

namespace swri_transform_util
{
/**
 * Transform a point from WGS84 lat/lon to an ortho-rectified LocalXY coordinate
 * system.
 *
 * @param[in] latitude             The input latitude in degrees.
 * @param[in] longitude            The input latitude in degrees.
 * @param[in] reference_latitude   The reference WGS84 latitude in degrees.
 * @param[in] reference_longitude  The reference WGS84 longitude in degrees.
 * @param[out] x                   The output X coordinate in meters.
 * @param[out] y                   The output Y coordinate in meters.
 */
  void LocalXyFromWgs84(
      double latitude,
      double longitude,
      double reference_latitude,
      double reference_longitude,
      double& x,
      double& y);

  /**
   * Transform a point from an ortho-rectified LocalXY coordinate system into
   * WGS84 latitude and longitude.
   *
   * Assumes the LocalXY data was generated with respect to the WGS84 datum.
   *
   * @param[in]  x                    The input X coordinate in meters.
   * @param[in]  y                    The input Y coordinate in meters.
   * @param[in]  reference_latitude   The reference WGS84 latitude in degrees.
   * @param[in]  reference_longitude  The reference WGS84 longitude in degrees.
   * @param[out] latitude             The output latitude in degrees.
   * @param[out] longitude            The output latitude in degrees.
   */
  void Wgs84FromLocalXy(
      double x,
      double y,
      double reference_latitude,
      double reference_longitude,
      double& latitude,
      double& longitude);

  /**
   * Utility class for converting between WGS84 lat/lon and an ortho-rectified
   * LocalXY coordinate system.
   */
  class LocalXyWgs84Util
  {
  public:
    /**
     * Constructor.
     *
     * @param[in] reference_latitude  Reference latitude in degrees.
     * @param[in] reference_longitude  Reference longitude in degrees.
     * @param[in] reference_altitude  Reference altitude in meters.
     * @param[in] reference_heading  Reference heading in degrees.
     */
    LocalXyWgs84Util(
        double reference_latitude,
        double reference_longitude,
        double reference_heading = 0,
        double reference_altitude = 0);
    // TODO(malban): What is the heading referenced from?

    LocalXyWgs84Util();

    bool Initialized() const { return initialized_; }

    double ReferenceLongitude() const;

    double ReferenceLatitude() const;

    double ReferenceHeading() const;

    double ReferenceAltitude() const;

    std::string Frame() const { return frame_; }

    /**
     * Convert WGS84 latitude and longitude to LocalXY.
     *
     * @param[in]  latitude   Latitude value in degrees.
     * @param[in]  longitude  Longitude value in degrees.
     * @param[out] x          X coordinate in meters from origin.
     * @param[out] y          Y coordinate in meters from origin.
     *
     * @returns True if the conversion is possible.
     */
    bool ToLocalXy(
        double latitude,
        double longitude,
        double& x,
        double& y) const;

    /**
     * Convert LocalXY to WGS84 latitude and longitude.
     *
     * @param[in]  x          X coordinate in meters from origin.
     * @param[in]  y          Y coordinate in meters from origin.
     * @param[out] latitude   Latitude value in degrees.
     * @param[out] longitude  Longitude value in degrees.
     *
     * @returns True if the conversion is possible.
     */
    bool ToWgs84(
        double x,
        double y,
        double& latitude,
        double& longitude) const;

  protected:
    double reference_latitude_;   //< Reference latitude in radians.
    double reference_longitude_;  //< Reference longitude in radians.
    double reference_heading_;    //< Reference heading in radians.
    double reference_altitude_;   //< Reference altitude in meters.

    double rho_lat_;
    double rho_lon_;
    double cos_heading_;
    double sin_heading_;

    std::string frame_;

    ros::Subscriber origin_sub_;
    bool initialized_;

    void Initialize();

    void HandleOrigin(const topic_tools::ShapeShifter::ConstPtr origin);
  };
  typedef boost::shared_ptr<LocalXyWgs84Util> LocalXyWgs84UtilPtr;
}

#endif  // TRANSFORM_UTIL_LOCAL_XY_UTIL_H_
