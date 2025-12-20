#include "Maths/Objects/Mat3x3.hpp"
#include "Maths/Objects/Vector2.hpp"

namespace Phezu {

	void Mat3x3::ResetToIdentity() {
		m_Matrix[0][0] = 0.0f;
		m_Matrix[0][1] = 0.0f;
		m_Matrix[0][2] = 0.0f;
		m_Matrix[1][0] = 0.0f;
		m_Matrix[1][1] = 0.0f;
		m_Matrix[1][2] = 0.0f;
		m_Matrix[2][0] = 0.0f;
		m_Matrix[2][1] = 0.0f;
		m_Matrix[2][2] = 0.0f;
	}

	void Mat3x3::SetTranslation(Vector2 translation) {
		m_Matrix[2][0] = translation.X();
		m_Matrix[2][1] = translation.Y();
	}

}