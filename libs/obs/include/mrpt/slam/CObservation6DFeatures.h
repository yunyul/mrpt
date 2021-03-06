/* +---------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)               |
   |                          http://www.mrpt.org/                             |
   |                                                                           |
   | Copyright (c) 2005-2014, Individual contributors, see AUTHORS file        |
   | See: http://www.mrpt.org/Authors - All rights reserved.                   |
   | Released under BSD License. See details in http://www.mrpt.org/License    |
   +---------------------------------------------------------------------------+ */
#ifndef CObservation6DFeatures_H
#define CObservation6DFeatures_H

#include <mrpt/utils/CSerializable.h>
#include <mrpt/slam/CObservation.h>
#include <mrpt/poses/CPose3D.h>

namespace mrpt
{
namespace slam
{
	DEFINE_SERIALIZABLE_PRE_CUSTOM_BASE_LINKAGE( CObservation6DFeatures, CObservation, OBS_IMPEXP  )

	/** An observation of one or more "features" or "objects", possibly identified with a unique ID, whose relative SE(3) pose is observed with respect to the sensor.
	 * The list of features is stored in \a sensedFeatures
	 * \sa CObservation
	 * \ingroup mrpt_obs_grp
	 */
	class OBS_IMPEXP CObservation6DFeatures : public CObservation
	{
		// This must be added to any CSerializable derived class:
		DEFINE_SERIALIZABLE( CObservation6DFeatures )
	 public:
		CObservation6DFeatures( );  //!< Default ctor

		float	minSensorDistance, maxSensorDistance;  //!< Information about the sensor

		/** Each one of the measurements */
		struct OBS_IMPEXP TMeasurement
		{
			mrpt::poses::CPose3D pose; //!< The observed feature SE(3) pose with respect to the sensor
			int32_t	             id;   //!< The feature ID, or INVALID_LANDMARK_ID if unidentified (default)
			mrpt::math::CMatrixDouble66 inf_matrix; //!< The inverse of the observation covariance matrix (default:all zeros)

			TMeasurement(); //!< Ctor with default values

			MRPT_MAKE_ALIGNED_OPERATOR_NEW  // Required because we contain Eigen matrices
		};
		std::deque<TMeasurement> sensedFeatures; //!< The list of observed features

		mrpt::poses::CPose3D sensorPose; //!< The pose of the sensor on the robot/vehicle

		// See base class docs.
		void getSensorPose( CPose3D &out_sensorPose ) const;
		// See base class docs.
		void setSensorPose( const CPose3D &newSensorPose );

	}; // End of class def.

	} // End of namespace
} // End of namespace

#endif
