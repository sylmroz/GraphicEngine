#pragma once

namespace GraphicEngine::Core
{
	template <typename BoudingBoxImpl, typename Vec>
	class BoundingBox
	{
	public:
		BoundingBox() {}
		BoundingBox(Vec left, Vec right):
			m_left{left},
			m_right{right}
		{
			extendBox(left);
			extendBox(right);
			recalculateCenter();
		}

		Vec getLeft()
		{
			return m_left;
		}

		void setLeft(Vec left)
		{
			m_left = left;
			recalculateCenter();
		}

		Vec getRight()
		{
			return m_right;
		}

		void setRight(Vec right)
		{
			m_right = right;
			recalculateCenter();
		}

		void extendBox(Vec p)
		{
			static_cast<BoudingBoxImpl*>(this)->recalculate(p);
			recalculateCenter();
		}


		void extendBox(BoudingBoxImpl p)
		{
			static_cast<BoudingBoxImpl*>(this)->recalculate(p.m_left);
			static_cast<BoudingBoxImpl*>(this)->recalculate(p.m_right);
			recalculateCenter();
		}

		Vec getCenter()
		{
			return m_center;
		}

		void setCenter(Vec p)
		{
			m_center = p;
		}

	protected:
		Vec m_left{};
		Vec m_right{};
		Vec m_center{};

		const float m_eplilion{ 0.000001f };

	private:
		void recalculateCenter()
		{
			m_center = (m_left + m_right) / 2.0f;
		}
	};
}