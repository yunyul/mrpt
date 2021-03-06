/* +---------------------------------------------------------------------------+
   |                     Mobile Robot Programming Toolkit (MRPT)               |
   |                          http://www.mrpt.org/                             |
   |                                                                           |
   | Copyright (c) 2005-2014, Individual contributors, see AUTHORS file        |
   | See: http://www.mrpt.org/Authors - All rights reserved.                   |
   | Released under BSD License. See details in http://www.mrpt.org/License    |
   +---------------------------------------------------------------------------+ */
#ifndef CPose3DQuatPDFGaussian_H
#define CPose3DQuatPDFGaussian_H

#include <mrpt/poses/CPose3DQuatPDF.h>
#include <mrpt/poses/CPose3DPDF.h>
#include <mrpt/poses/CPosePDF.h>
#include <mrpt/math/CMatrixD.h>

namespace mrpt
{
namespace poses
{
	class CPosePDFGaussian;
	class CPose3DPDFGaussian;

	DEFINE_SERIALIZABLE_PRE_CUSTOM_BASE( CPose3DQuatPDFGaussian , CPose3DQuatPDF )

	/** Declares a class that represents a Probability Density function (PDF) of a 3D pose using a quaternion \f$ p(\mathbf{x}) = [x ~ y ~ z ~ qr ~ qx ~ qy ~ qz]^\top \f$.
	 *
	 *   This class implements that PDF using a mono-modal Gaussian distribution. See mrpt::poses::CPose3DQuatPDF for more details, or
	 *    mrpt::poses::CPose3DPDF for classes based on Euler angles instead.
	 *
	 *  Uncertainty of pose composition operations (\f$ y = x \oplus u \f$) is implemented in the methods "CPose3DQuatPDFGaussian::operator+=" and "CPose3DQuatPDF::jacobiansPoseComposition".
	 *
	 *  For further details on implemented methods and the theory behind them,
	 *  see <a href="http://www.mrpt.org/6D_poses:equivalences_compositions_and_uncertainty" >this report</a>.
	 *
	 * \sa CPose3DQuat, CPose3DQuatPDF, CPose3DPDF, CPose3DQuatPDFGaussianInf
	 * \ingroup poses_pdf_grp
	 */
	class BASE_IMPEXP CPose3DQuatPDFGaussian : public CPose3DQuatPDF
	{
		// This must be added to any CSerializable derived class:
		DEFINE_SERIALIZABLE( CPose3DQuatPDFGaussian )

	protected:
		/** Assures the symmetry of the covariance matrix (eventually certain operations in the math-coprocessor lead to non-symmetric matrixes!)
		  */
		void  assureSymmetry();

	 public:
		 /** Default constructor - set all values to zero. */
		CPose3DQuatPDFGaussian();

		/** Constructor which left all the member uninitialized, for using when speed is critical - as argument, use UNINITIALIZED_QUATERNION. */
		CPose3DQuatPDFGaussian(mrpt::math::TConstructorFlags_Quaternions constructor_dummy_param);

		/** Constructor from a default mean value, covariance equals to zero. */
		explicit CPose3DQuatPDFGaussian( const CPose3DQuat &init_Mean );

		/** Constructor with mean and covariance. */
		CPose3DQuatPDFGaussian( const CPose3DQuat &init_Mean, const mrpt::math::CMatrixDouble77 &init_Cov );

		/** Constructor from a Gaussian 2D pose PDF (sets to 0 the missing variables). */
		explicit CPose3DQuatPDFGaussian( const CPosePDFGaussian &o );

		/** Constructor from an equivalent Gaussian in Euler angles representation. */
		explicit CPose3DQuatPDFGaussian( const CPose3DPDFGaussian &o );

		/** The mean value */
		CPose3DQuat		mean;

		/** The 7x7 covariance matrix */
		mrpt::math::CMatrixDouble77		cov;

		inline const CPose3DQuat & getPoseMean() const { return mean; }
		inline       CPose3DQuat & getPoseMean()       { return mean; }

		 /** Returns an estimate of the pose, (the mean, or mathematical expectation of the PDF).
		   * \sa getCovariance
		   */
		void getMean(CPose3DQuat &mean_pose) const;

		/** Returns an estimate of the pose covariance matrix (7x7 cov matrix) and the mean, both at once.
		  * \sa getMean
		  */
		void getCovarianceAndMean(mrpt::math::CMatrixDouble77 &cov,CPose3DQuat &mean_point) const;

		/** Copy operator, translating if necesary (for example, between particles and gaussian representations)
		  */
		void  copyFrom(const CPose3DQuatPDF &o);

		/** Copy operator, translating if necesary (for example, between particles and gaussian representations)
		  */
		void  copyFrom(const CPosePDF &o);

		/** Copy operator, translating if necesary (for example, between particles and gaussian representations)
		  */
		void  copyFrom(const CPose3DPDFGaussian &o);

		/** Save the PDF to a text file, containing the 3D pose in the first line (x y z qr qx qy qz), then the covariance matrix in the next 7 lines.
		 */
		void  saveToTextFile(const std::string &file) const;

		/** this = p (+) this. This can be used to convert a PDF from local coordinates to global, providing the point (newReferenceBase) from which
		  *   "to project" the current pdf. Result PDF substituted the currently stored one in the object.
		  */
		void  changeCoordinatesReference(  const CPose3DQuat &newReferenceBase );

		/** this = p (+) this. This can be used to convert a PDF from local coordinates to global, providing the point (newReferenceBase) from which
		  *   "to project" the current pdf. Result PDF substituted the currently stored one in the object.
		  */
		void  changeCoordinatesReference(  const CPose3D &newReferenceBase );

		/** Draws a single sample from the distribution
		  */
		void  drawSingleSample( CPose3DQuat &outPart ) const;

		/** Draws a number of samples from the distribution, and saves as a list of 1x7 vectors, where each row contains a (x,y,z,qr,qx,qy,qz) datum.
		  */
		void  drawManySamples( size_t N, std::vector<CVectorDouble> & outSamples ) const;

		/** Returns a new PDF such as: NEW_PDF = (0,0,0) - THIS_PDF
		  */
		void	 inverse(CPose3DQuatPDF &o) const;

		/** Unary - operator, returns the PDF of the inverse pose.  */
		inline CPose3DQuatPDFGaussian operator -() const
		{
			CPose3DQuatPDFGaussian p(mrpt::math::UNINITIALIZED_QUATERNION);
			this->inverse(p);
			return p;
		}

		/** Makes: thisPDF = thisPDF + Ap, where "+" is pose composition (both the mean, and the covariance matrix are updated).
		  */
		void  operator += ( const CPose3DQuat &Ap);

		/** Makes: thisPDF = thisPDF + Ap, where "+" is pose composition (both the mean, and the covariance matrix are updated) (see formulas in jacobiansPoseComposition ).
		  */
		void  operator += ( const CPose3DQuatPDFGaussian &Ap);

		/** Makes: thisPDF = thisPDF - Ap, where "-" is pose inverse composition (both the mean, and the covariance matrix are updated).
		  */
		void  operator -= ( const CPose3DQuatPDFGaussian &Ap);

		/** Evaluates the PDF at a given point.
		  */
		double  evaluatePDF( const CPose3DQuat &x ) const;

		/** Evaluates the ratio PDF(x) / PDF(MEAN), that is, the normalized PDF in the range [0,1].
		  */
		double  evaluateNormalizedPDF( const CPose3DQuat &x ) const;

		/** Computes the Mahalanobis distance between the centers of two Gaussians.
		  *  The variables with a variance exactly equal to 0 are not taken into account in the process, but
		  *   "infinity" is returned if the corresponding elements are not exactly equal.
		  */
		double  mahalanobisDistanceTo( const CPose3DQuatPDFGaussian& theOther);

	}; // End of class def.


	/** Pose composition for two 3D pose Gaussians  \sa CPose3DQuatPDFGaussian::operator += */
	inline CPose3DQuatPDFGaussian operator +( const CPose3DQuatPDFGaussian &x, const CPose3DQuatPDFGaussian &u )
	{
		CPose3DQuatPDFGaussian 	res(x);
		res+=u;
		return res;
	}

	/** Inverse pose composition for two 3D pose Gaussians  \sa CPose3DQuatPDFGaussian::operator -= */
	inline CPose3DQuatPDFGaussian operator -( const CPose3DQuatPDFGaussian &x, const CPose3DQuatPDFGaussian &u )
	{
		CPose3DQuatPDFGaussian 	res(x);
		res-=u;
		return res;
	}

	/** Dumps the mean and covariance matrix to a text stream.
	  */
	std::ostream  BASE_IMPEXP & operator << (std::ostream & out, const CPose3DQuatPDFGaussian& obj);

	bool BASE_IMPEXP operator==(const CPose3DQuatPDFGaussian &p1,const CPose3DQuatPDFGaussian &p2);

	} // End of namespace

	namespace global_settings
	{
		/** If set to true (default), a Scaled Unscented Transform is used instead of a linear approximation with Jacobians.
		  * Affects to:
		  *		- CPose3DQuatPDFGaussian::copyFrom(const CPose3DPDFGaussian &o)
		  *		- CPose3DQuatPDFGaussianInf::copyFrom(const CPose3DPDFGaussianInf &o)
		  */
		extern BASE_IMPEXP bool USE_SUT_EULER2QUAT_CONVERSION;
	}

} // End of namespace

#endif
