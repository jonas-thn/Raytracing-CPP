#pragma once

#include "../qbLinAlg/qbVector.h"
#include "../qbLinAlg/qbMatrix.h"
#include "Ray.h"

namespace RT
{
	//direction flag values
	constexpr bool FWDTFORM = true;
	constexpr bool BCKTFORM = false;

	class GTFM
	{
	public:
		GTFM();
		~GTFM();

		GTFM(const qbVector<double>& translation, const qbVector<double>& rotation, const qbVector<double>& scale);

		GTFM(const qbMatrix2<double>& fwd, const qbMatrix2<double>& bck);

		void SetTransform(const qbVector<double>& translation, const qbVector<double>& rotation, const qbVector<double>& scale);
		qbMatrix2<double> GetForward();
		qbMatrix2<double> GetBackward();

		RT::Ray Apply(const RT::Ray& inputRay, bool dirFlag);
		qbVector<double> Apply(const qbVector<double>& inputVector, bool dirFlag);

		//overload operators
		friend GTFM operator* (const RT::GTFM& lhs, const RT::GTFM& rhs);
		GTFM operator=(const GTFM& rhs);

		void PrintMatrix(bool dirFlag);

		static void PrintVector(const qbVector<double>& vector);

	private:
		void Print(const qbMatrix2<double>& matrix);

	private:
		qbMatrix2<double> m_fwdtfm{ 4, 4 };
		qbMatrix2<double> m_bcktfm{ 4, 4 };
	};
}



