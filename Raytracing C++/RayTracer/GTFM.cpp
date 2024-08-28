#include "GTFM.h"

RT::GTFM::GTFM()
{
	m_fwdtfm.SetToIdentity();
	m_bcktfm.SetToIdentity();
}

RT::GTFM::~GTFM()
{

}

RT::GTFM::GTFM(const qbMatrix2<double>& fwd, const qbMatrix2<double>& bck)
{
	if ((fwd.GetNumRows()) != 4 || (fwd.GetNumCols() != 4) || (bck.GetNumRows() != 4) || (bck.GetNumCols() != 4))
	{
		throw std::invalid_argument("Cannot construct GTFM, inputs are not 4x4");
	}

	m_fwdtfm = fwd;
	m_bcktfm = bck;
}

void RT::GTFM::SetTransform(const qbVector<double>& translation, const qbVector<double>& rotation, const qbVector<double>& scale)
{
	qbMatrix2<double> translationMatrix{ 4, 4 };
	qbMatrix2<double> rotataionMatrixX{ 4, 4 };
	qbMatrix2<double> rotataionMatrixY{ 4, 4 };
	qbMatrix2<double> rotataionMatrixZ{ 4, 4 };
	qbMatrix2<double> scaleMatrix{ 4, 4 };

	translationMatrix.SetToIdentity();
	rotataionMatrixX.SetToIdentity();
	rotataionMatrixY.SetToIdentity();
	rotataionMatrixZ.SetToIdentity();
	scaleMatrix.SetToIdentity();

	//translationMatrix
	translationMatrix.SetElement(0, 3, translation.GetElement(0));
	translationMatrix.SetElement(1, 3, translation.GetElement(1));
	translationMatrix.SetElement(2, 3, translation.GetElement(2));

	//roatationMatrix
	rotataionMatrixZ.SetElement(0, 0, cos(rotation.GetElement(2)));
	rotataionMatrixZ.SetElement(0, 1, sin(rotation.GetElement(2)));
	rotataionMatrixZ.SetElement(1, 0, -sin(rotation.GetElement(2)));
	rotataionMatrixZ.SetElement(1, 1, cos(rotation.GetElement(2)));

	rotataionMatrixY.SetElement(0, 0, cos(rotation.GetElement(1)));
	rotataionMatrixY.SetElement(0, 2, -sin(rotation.GetElement(1)));
	rotataionMatrixY.SetElement(2, 0, sin(rotation.GetElement(1)));
	rotataionMatrixY.SetElement(2, 2, cos(rotation.GetElement(1)));

	rotataionMatrixX.SetElement(1, 1, cos(rotation.GetElement(0)));
	rotataionMatrixX.SetElement(1, 2, sin(rotation.GetElement(0)));
	rotataionMatrixX.SetElement(2, 1, -sin(rotation.GetElement(0)));
	rotataionMatrixX.SetElement(2, 2, cos(rotation.GetElement(0)));

	//scaleMatrix
	scaleMatrix.SetElement(0, 0, scale.GetElement(0));
	scaleMatrix.SetElement(1, 1, scale.GetElement(1));
	scaleMatrix.SetElement(2, 2, scale.GetElement(2));

	m_fwdtfm = translationMatrix * scaleMatrix * rotataionMatrixX * rotataionMatrixX * rotataionMatrixZ;

	m_bcktfm = m_fwdtfm;
	m_bcktfm.Inverse();
}

qbMatrix2<double> RT::GTFM::GetForward()
{
	return m_fwdtfm;
}

qbMatrix2<double> RT::GTFM::GetBackward()
{
	return m_bcktfm;
}

RT::Ray RT::GTFM::Apply(const RT::Ray& inputRay, bool dirFlag)
{
	RT::Ray outputRay;

	if (dirFlag)
	{
		outputRay.m_point1 = this->Apply(inputRay.m_point1, RT::FWDTFORM);
		outputRay.m_point2 = this->Apply(inputRay.m_point2, RT::FWDTFORM);
		outputRay.m_lab = outputRay.m_point2 - outputRay.m_point1;

	}
	else
	{
		outputRay.m_point1 = this->Apply(inputRay.m_point1, RT::BCKTFORM);
		outputRay.m_point2 = this->Apply(inputRay.m_point2, RT::BCKTFORM);
		outputRay.m_lab = outputRay.m_point2 - outputRay.m_point1;
	}

	return outputRay;
}

qbVector<double> RT::GTFM::Apply(const qbVector<double>& inputVector, bool dirFlag)
{
	std::vector<double> tempData{ inputVector.GetElement(0), inputVector.GetElement(1), inputVector.GetElement(2), 1.0 };
	qbVector<double> tempVector{ tempData };
	qbVector<double> resultVector;

	if (dirFlag)
	{
		//apply forward transform
		resultVector = m_fwdtfm * tempVector;
	}
	else
	{
		//apply backward transform
		resultVector = m_bcktfm * tempVector;
	}

	qbVector<double> outputVector {std::vector<double> {resultVector.GetElement(0), resultVector.GetElement(1), resultVector.GetElement(2)}};

	return outputVector;
}

RT::GTFM RT::GTFM::operator=(const GTFM& rhs)
{
	if (this != &rhs)
	{
		m_fwdtfm = rhs.m_fwdtfm;
		m_bcktfm = rhs.m_bcktfm;
	}

	return *this;
}

void RT::GTFM::PrintMatrix(bool dirFlag)
{
	if (dirFlag)
	{
		Print(m_fwdtfm);
	}
	else
	{
		Print(m_bcktfm);
	}
}

void RT::GTFM::PrintVector(const qbVector<double>& vector)
{
	int nRows = vector.GetNumDims();
	for (int row = 0; row < nRows; ++row)
	{
		std::cout << std::fixed << std::setprecision(3) << vector.GetElement(row) << std::endl;
	}
}

void RT::GTFM::Print(const qbMatrix2<double>& matrix)
{
	int nRows = matrix.GetNumRows();
	int nCols = matrix.GetNumCols();

	for (int row = 0; row < nRows; ++row)
	{
		for (int col = 0; col < nCols; ++col)
		{
			std::cout << std::fixed << std::setprecision(3) << matrix.GetElement(row, col) << " ";
		}

		std::cout << std::endl;
	}
}

namespace RT
{
	GTFM RT::operator*(const RT::GTFM& lhs, const RT::GTFM& rhs)
	{
		qbMatrix2<double> fwdResult = lhs.m_fwdtfm * rhs.m_fwdtfm;

		//backwards transform as inverse of forward transform
		qbMatrix2<double> bckResult = fwdResult;
		bckResult.Inverse();

		//final result
		RT::GTFM finalResult(fwdResult, bckResult);

		return finalResult;
	}
}

