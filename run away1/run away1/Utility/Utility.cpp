//#include <string>
//#include <iostream>
//#include <iomanip>
//#include <fstream>
//#include <sstream>
//#include <vector>
//#include <math.h>
//#include <DxLib.h>
//#include "Utility.h"
//
//int OneShotUtility::textSize(std::string text, int fontHandle)
//{
//    int len = (int)strlen(text.c_str());
//
//    if (fontHandle == -1) {
//        return GetDrawStringWidth(text.c_str(), len);
//    }
//    return GetDrawStringWidthToHandle(text.c_str(), len, fontHandle);
//}
//
//std::vector<std::string> OneShotUtility::Split(const std::string& str, char delimiter) {
//    std::vector<std::string> result;
//    std::stringstream ss(str);
//    std::string item;
//
//    while (std::getline(ss, item, delimiter)) {
//        result.push_back(item);
//    }
//
//    return result;
//}
//
//double OneShotUtility::Rad2DegD(double rad)
//{
//    return rad * (180.0 / DX_PI);
//}
//
//float OneShotUtility::Rad2DegF(float rad)
//{
//    return rad * (180.0f / DX_PI_F);
//}
//
//
//float OneShotUtility::AngleDegVector2(const Vector2& v1, const Vector2& v2)
//{
//    float dot = Dot(v1, v2);
//    float det = Det(v1, v2);
//
//    float angle = atan2f(dot, det);
//
//    angle = Rad2DegF(angle);
//
//
//
//    if (angle < 0) {
//        angle += 360.0f;
//    }
//
//    return angle;
//}
//
//bool OneShotUtility::CrossVector2(const Vector2& pv1, const Vector2& av1, const Vector2& pv2, const Vector2& av2)
//{
//    //v2‚ðŒ³‚ÉŒvŽZ‚·‚é
//    //ŠOÏŒvŽZ
//    float angle1 = OneShotUtility::Det(pv1 - pv2, av2 - pv2);
//    float angle2 = OneShotUtility::Det(av1 - pv2, av2 - pv2);
//
//    if ((angle1 >= 0 && angle2 <= 0) || (angle1 <= 0 && angle2 >= 0)) {
//
//        angle1 = OneShotUtility::Det(pv2 - pv1, av1 - pv1);
//        angle2 = OneShotUtility::Det(av2 - pv1, av1 - pv1);
//
//        if ((angle1 > 0 && angle2 < 0) || (angle1 < 0 && angle2 > 0)) {
//            return true;
//        }
//    }
//    return false;
//}
//
//Vector2 OneShotUtility::CrossPointVector2(const Vector2& pv1, const Vector2& av1, const Vector2& pv2, const Vector2& av2)
//{
//    float area1 = OneShotUtility::Det(pv1 - pv2, av2 - pv2);
//    if (area1 < 0) {
//        area1 *= -1;
//    }
//    float area2 = OneShotUtility::Det(av1 - pv2, av2 - pv2);
//    if (area2 < 0) {
//        area2 *= -1;
//    }
//
//    area1 = std::trunc(area1);
//    area2 = std::trunc(area2);
//
//    return pv1 + av1 * (area1 / (area1 * area2));
//}
//
//std::string OneShotUtility::Padding(const int val, const int num, const char set)
//{
//    //ƒXƒRƒA‚Ì”Žš‚ª‚²•¶Žš‚É‚É‚È‚é‚æ‚¤‚É‚O‚ð“ü‚ê‚é
//    std::string S = std::to_string(val);
//    std::ostringstream result;
//    result << std::setfill(set) << std::setw(num) << S;
//    //Œ‹‰Ê‚ð“ü‚ê‚é
//    return result.str();
//}
//#include "Utility.h"
