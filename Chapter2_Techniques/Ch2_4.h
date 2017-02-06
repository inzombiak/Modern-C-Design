#ifndef CH_2_4_H
#define CH_2_4_H

/*
2.4. Mapping Integral Constants to Types
Allows us to do compile-time checks on what function to call

TODO: Merge with Policies
*/
namespace Chapter2
{

	template <int v>
	struct Int2Type
	{
		static const int value = v;
	};

	namespace Ch2_4
	{
		void Run();

		enum WallType
		{
			GlassWall = 0,
			BrickWall = 1,
			WoodWall = 2,
		};

		class Glass
		{
		public:
			void Break();
		};

		class Brick
		{
		public:
			void BreakFist();
		};

		class Wood
		{
		public:
			void BreakBoth();			
		};

		template <typename T, WallType isBreakable>
		class Wall
		{
		public:
			void Hit(T* obj);
		private:
			void Hit(T* obj, Int2Type<GlassWall>)
			{
				obj->Break();
			}

			void Hit(T* obj, Int2Type<BrickWall>)
			{
				obj->BreakFist();
			}

			void Hit(T* obj, Int2Type<WoodWall>)
			{
				obj->BreakBoth();
			}
		};


	}
}



/*
End 2.4.
*/

#endif