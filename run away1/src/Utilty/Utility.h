#pragma once
#include <string> 
#include <vector>
#include "../Common/Vector2.h"

class OneShotUtility
{
public:


	struct Point
	{
		float x, y;
	};

	//•¶Í‚Ì‘å‚«‚³‚ğ}‚éŠÖ”
	static int textSize(std::string text,int fontHandle=-1);

	static std::vector<std::string> Split(const std::string& str, char delimiter);

	//“àÏ‚ÌŒvZ
	static float Dot(const Vector2& v1, const Vector2& v2) {
		return v1.x * v2.x + v1.y * v2.y ;
	}
	//ŠOÏ‚ÌŒvZ
	static float Det(const Vector2& v1, const Vector2& v2) {
		return v1.x * v2.y - v1.y * v2.x;

	}

	// ƒ‰ƒWƒAƒ“(rad)‚©‚ç“x(deg)
	static double Rad2DegD(double rad);
	static float Rad2DegF(float rad);

	static float AngleDegVector2(const Vector2& v1, const Vector2& v2 );

	//Œğ·”»’èi‚S‚Â‚Ì“_‚©‚çŒğ·”»’ès‚¤
	static bool CrossVector2(const Vector2& pv1, const Vector2& av1,
							const Vector2& pv2, const Vector2& av2);
	//—^‚¦‚ç‚ê‚½4‚Á‚Â‚Ì“_‚©‚çŒğ·“_‚ğŒ©‚Â‚¯‚Ä•Ô‚·
	static Vector2 CrossPointVector2(const Vector2& pv1, const Vector2& av1,
		const Vector2& pv2, const Vector2& av2);

	//—^‚¦‚ç‚ê‚½”š‚ğˆø”‚Q‚Æ“¯‚¶”‚É‚È‚é‚Ü‚Åˆø”‚R‚Ì”‚ğ“ü‚ê‚éŠÖ”
	static std::string  Padding(const int val, const int num, const char set ='0');
};

