#pragma once

namespace GraphicEngine::Engines::Graphic
{
	template <typename BoudingBoxImpl, typename Vec>
	class BoundingBox
	{
	public:
		BoundingBox() {}
		BoundingBox(Vec left, Vec right):
			m_left{left},
			m_right{right}
		{}

		Vec getLeft()
		{
			return m_left;
		}

		void setLeft(Vec left)
		{
			m_left = left;
		}

		Vec getRight()
		{
			return m_right;
		}

		void setRight(Vec right)
		{
			m_right = right;
		}

		void extendBox(Vec p)
		{
			static_cast<BoudingBoxImpl*>(this)->recalculate(p);
		}

		Vec getCenter()
		{
			return (m_left + m_right) / 2.0f;
		}

	protected:
		Vec m_left;
		Vec m_right;
	};
}