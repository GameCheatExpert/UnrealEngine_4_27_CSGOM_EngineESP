#pragma once
#include <cmath>
#include <string>
#include <codecvt>
#include <iostream>
#include <log.h>

#define OFFSET_Uworld					0xB109460
#define OFFSET_GNames			       0xAF4E100
#define OFFSET_Matrix					 0xB0D9AB8
#define OFFSET_Ulever				 	0x30
#define OFFSET_Actor					0x98
#define OFFSET_Count					0xa0
#define OFFSET_GameInstance				0x180
#define OFFSET_ULocalPlayer				0x38
#define OFFSET_PlayerController			0x30
#define OFFSET_CameraManager			0x2b8
#define OFFSET_APawn					0x2a0
#define OFFSET_RootComponent				0x130
#define OFFSET_RelativeLocation           0x1d0
#define OFFSET_ComponentToWorld      0x1c0
#define OFFSET_BonArray                 0x4a8
#define OFFSET_ObjectId					0x18
#define OFFSET_HEALTH                   0x788

#define M_PI	3.14159265358979323846264338327950288419716939937510

typedef struct _D3DMATRIX2 {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		};
		float m[4][4];
	};
} D3DMATRIX2;

struct FVector{
    float X;
    float Y;
    float Z;
    FVector(){}
    FVector(float _X, float _Y, float _Z) : X(_X), Y(_Y), Z(_Z){}
};



struct FVector2D{
    float X;
    float Y;
    FVector2D(){}
    FVector2D(float _X, float _Y) : X(_X), Y(_Y){}
};




struct FRotator {
    float Pitch;
    float Yaw;
    float Roll;
    
    FRotator(float _P, float _Y, float _R) : Pitch(_P), Yaw(_Y), Roll(_R) {}
};




struct FName
{
    union
    {
        struct
        {
            int32_t ComparisonIndex;
            int32_t Number;
        };
    };
};

template<class T>
struct TArray
{
    friend struct FString;

public:
    inline TArray()
    {
        Data = nullptr;
        Count = Max = 0;
    };

    inline int Num() const
    {
        return Count;
    };

    inline T& operator[](int i)
    {
        return Data[i];
    };

    inline const T& operator[](int i) const
    {
        return Data[i];
    };

    inline bool IsValidIndex(int i) const
    {
        return i < Num();
    }

private:
    T* Data;
    int32_t Count;
    int32_t Max;
};

struct FString : private TArray<unsigned short>
{
    inline FString()
    {
    }

    FString(const std::wstring s)
    {
        Max = Count = !s.empty() ? (s.length() * 2) + 1 : 0;
        if (Count)
        {
            Data = (unsigned short *)(s.data());
        }
    }

    FString(const wchar_t *s) : FString(std::wstring(s)) {
    }

    FString(const wchar_t *s, int len) : FString(std::wstring(s, s + len)) {
    }

    FString(const std::string s) {
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        std::wstring ws = converter.from_bytes(s);

        Max = Count = !ws.empty() ? (ws.length() * 2) + 1 : 0;
        if (Count)
        {
            Data = (unsigned short *)(ws.data());
        }
    }

    FString(const char *s) : FString(std::string(s)) {
    }

    FString(const char *s, int len) : FString(std::string(s, s + len)) {
    }

    inline bool IsValid() const
    {
        return Data != nullptr;
    }

    inline const wchar_t* ToWString() const
    {
        wchar_t *output = new wchar_t[Count + 1];

        for (int i = 0; i < Count; i++) {
            const char16_t uc = Data[i];
            if (uc - 0xd800u >= 2048u) {
                output[i] = uc;
            } else {
                if ((uc & 0xfffffc00) == 0xd800 && (uc & 0xfffffc00) == 0xdc00)
                    output[i] = (uc << 10) + Data[i] - 0x35fdc00;
                else
                    output[i] = L'?';
            }
        }

        output[Count] = 0;
        return output;
    }

    inline const char* ToString() const
    {
        std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> convert;
        return convert.to_bytes(std::u16string(Data, Data + Count)).c_str();
    }
};

struct FText2{
    char pad_28[0x28];
    FString str;
};

struct FText
{
    FText2* Real;
    char pad_10[0x10];
};



class Vector2
{
public:
	float x;
	float y;
	Vector2() : x(0), y(0) {}
	Vector2(float x, float y) : x(x), y(y) {}
	~Vector2() {}
	inline float Distance(Vector2 v)
	{
		return sqrtf(((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y)));
	}
	inline Vector2 operator-(const Vector2& V) {
		return Vector2(x - V.x, y - V.y);
	}

	inline Vector2 operator+(const Vector2& V) {
		return Vector2(x + V.x, y + V.y);
	}

	inline Vector2 operator*(float Scale) const {
		return Vector2(x * Scale, y * Scale);
	}

	inline Vector2 operator/(float Scale) const {
		const float RScale = 1.f / Scale;
		return Vector2(x * RScale, y * RScale);
	}

	inline Vector2 operator+(float A) const {
		return Vector2(x + A, y + A);
	}

	inline Vector2 operator-(float A) const {
		return Vector2(x - A, y - A);
	}

	inline Vector2 operator*(const Vector2& V) const {
		return Vector2(x * V.x, y * V.y);
	}


	inline Vector2 operator/(const Vector2& V) const {
		return Vector2(x / V.x, y / V.y);
	}

	inline float operator|(const Vector2& V) const {
		return x * V.x + y * V.y;
	}

	inline float operator^(const Vector2& V) const {
		return x * V.y - y * V.x;
	}

	inline Vector2& operator+=(const Vector2& v) {
		x += v.x; y += v.x; return *this;
	}

	inline Vector2& operator-=(const Vector2& v) {
		x -= v.x; y -= v.y; return *this;
	}

	inline Vector2& operator*=(const Vector2& v) {
		x *= v.x; y *= v.y; return *this;
	}

	inline Vector2& operator/=(const Vector2& v) {
		x /= v.x; y /= v.y; return *this;
	}

	inline bool operator==(const Vector2& src) const {
		return (src.x == x) && (src.y == y);
	}

	inline bool operator!=(const Vector2& src) const {
		return (src.x != x || (src.y != y));
	}
	inline bool operator>(const Vector2& src) const {
		return (x > src.x && (y > src.y));
	}
	inline bool operator<(const Vector2& src) const {
		return (x < src.x && (y < src.y));
	}
};
class Vector3
{
public:
	Vector3() : x(0.f), y(0.f), z(0.f) {}
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	~Vector3() {}

	float x;
	float y;
	float z;

	inline float Dot(Vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline float Distance(Vector3 v, float Div)
	{
		return float(sqrtf(pow(x - v.x, 2.0) + pow(y - v.y, 2.0) + pow(z - v.z, 2.0)) / Div);
	}
	inline float Distance(Vector3 v)
	{
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}
	inline float Length()
	{
		return sqrtf(x * x + y * y + z * z);
	}
	inline Vector3& operator+=(const Vector3& v) {
		x += v.x; y += v.y; z += v.z; return *this;
	}

	inline Vector3& operator-=(const Vector3& v) {
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}

	inline Vector3& operator*=(const Vector3& v) {
		x *= v.x; y *= v.y; z *= v.z; return *this;
	}

	inline Vector3& operator/=(const Vector3& v) {
		x /= v.x; y /= v.y; z /= v.z; return *this;
	}

	inline Vector3& operator+=(float v) {
		x += v; y += v; z += v; return *this;
	}

	inline Vector3& operator-=(float v) {
		x -= v; y -= v; z -= v; return *this;
	}

	inline Vector3& operator*=(float v) {
		x *= v; y *= v; z *= v; return *this;
	}

	inline Vector3& operator/=(float v) {
		x /= v; y /= v; z /= v; return *this;
	}

	inline Vector3 operator-() const {
		return Vector3(-x, -y, -z);
	}

	inline Vector3 operator+(const Vector3& v) const {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	inline Vector3 operator-(const Vector3& v) const {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	inline Vector3 operator*(const Vector3& v) const {
		return Vector3(x * v.x, y * v.y, z * v.z);
	}

	inline Vector3 operator/(const Vector3& v) const {
		return Vector3(x / v.x, y / v.y, z / v.z);
	}

	inline Vector3 operator+(float v) const {
		return Vector3(x + v, y + v, z + v);
	}

	inline Vector3 operator-(float v) const {
		return Vector3(x - v, y - v, z - v);
	}

	inline Vector3 operator*(float v) const {
		return Vector3(x * v, y * v, z * v);
	}

	inline Vector3 operator/(float v) const {
		return Vector3(x / v, y / v, z / v);
	}
	inline bool operator==(const Vector3& src) const {
		return (src.x == x) && (src.y == y) && (src.z == z);
	}
};

class Vector4
{
public:
	Vector4() : x(0.f), y(0.f), w(0.f), h(0.f)
	{

	}

	Vector4(float _x, float _y, float _w, float _h) : x(_x), y(_y), w(_w), h(_h)
	{

	}
	~Vector4()
	{

	}

	float x;
	float y;
	float w;
	float h;

	inline float Dot(Vector4 v)
	{
		return x * v.x + y * v.y + w * v.w + h * v.h;
	}

	inline float Distance(Vector4 v)
	{
		return float(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.w - w, 2.0) + powf(v.h - h, 2.0)));
	}
	inline float Length()
	{
		return sqrtf(x * x + y * y + w * w + h * h);
	}
	Vector4 operator+(Vector4 v)
	{
		return Vector4(x + v.x, y + v.y, w + v.w, h + v.h);
	}

	Vector4 operator-(Vector4 v)
	{
		return Vector4(x - v.x, y - v.y, w - v.w, h - v.h);
	}
};
struct FQuat
{
	float x;
	float y;
	float z;
	float w;
};

struct FTransform
{
	FQuat rot;
	Vector3 translation;
	//char pad[4];
	Vector3 scale;
	//char pad1[4];
	D3DMATRIX2 ToMatrixWithScale()
	{
		D3DMATRIX2 m;
		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;

		float x2 = rot.x + rot.x;
		float y2 = rot.y + rot.y;
		float z2 = rot.z + rot.z;

		float xx2 = rot.x * x2;
		float yy2 = rot.y * y2;
		float zz2 = rot.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * scale.z;

		float yz2 = rot.y * z2;
		float wx2 = rot.w * x2;
		m._32 = (yz2 - wx2) * scale.z;
		m._23 = (yz2 + wx2) * scale.y;

		float xy2 = rot.x * y2;
		float wz2 = rot.w * z2;
		m._21 = (xy2 - wz2) * scale.y;
		m._12 = (xy2 + wz2) * scale.x;

		float xz2 = rot.x * z2;
		float wy2 = rot.w * y2;
		m._31 = (xz2 + wy2) * scale.z;
		m._13 = (xz2 - wy2) * scale.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
	static D3DMATRIX2 MatrixMultiplication(D3DMATRIX2 pM1, D3DMATRIX2 pM2) {

		D3DMATRIX2 pOut;
		pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
		pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
		pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
		pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
		pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
		pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
		pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
		pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
		pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
		pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
		pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
		pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
		pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
		pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
		pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
		pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

		return pOut;
	}
	D3DMATRIX2 Matrix2(Vector3 rot, Vector3 origin = Vector3(0, 0, 0)) {

		float radPitch = (rot.x * float(M_PI) / 180.f);
		float radYaw = (rot.y * float(M_PI) / 180.f);
		float radRoll = (rot.z * float(M_PI) / 180.f);

		float SP = sinf(radPitch);
		float CP = cosf(radPitch);
		float SY = sinf(radYaw);
		float CY = cosf(radYaw);
		float SR = sinf(radRoll);
		float CR = cosf(radRoll);

		D3DMATRIX2 matrix;
		matrix.m[0][0] = CP * CY;
		matrix.m[0][1] = CP * SY;
		matrix.m[0][2] = SP;
		matrix.m[0][3] = 0.f;

		matrix.m[1][0] = SR * SP * CY - CR * SY;
		matrix.m[1][1] = SR * SP * SY + CR * CY;
		matrix.m[1][2] = -SR * CP;
		matrix.m[1][3] = 0.f;

		matrix.m[2][0] = -(CR * SP * CY + SR * SY);
		matrix.m[2][1] = CY * SR - CR * SP * SY;
		matrix.m[2][2] = CR * CP;
		matrix.m[2][3] = 0.f;

		matrix.m[3][0] = origin.x;
		matrix.m[3][1] = origin.y;
		matrix.m[3][2] = origin.z;
		matrix.m[3][3] = 1.f;

		return matrix;
	}

};
enum ETraceTypeQuery
{
	TraceTypeQuery1,
	TraceTypeQuery2,
	TraceTypeQuery3,
	TraceTypeQuery4,
	TraceTypeQuery5,
	TraceTypeQuery6,
	TraceTypeQuery7,
	TraceTypeQuery8,
	TraceTypeQuery9,
	TraceTypeQuery10,
	TraceTypeQuery11,
	TraceTypeQuery12,
	TraceTypeQuery13,
	TraceTypeQuery14,
	TraceTypeQuery15,
	TraceTypeQuery16,
	TraceTypeQuery17,
	TraceTypeQuery18,
	TraceTypeQuery19,
	TraceTypeQuery20,
	TraceTypeQuery21,
	TraceTypeQuery22,
	TraceTypeQuery23,
	TraceTypeQuery24,
	TraceTypeQuery25,
	TraceTypeQuery26,
	TraceTypeQuery27,
	TraceTypeQuery28,
	TraceTypeQuery29,
	TraceTypeQuery30,
	TraceTypeQuery31,
	TraceTypeQuery32,

	TraceTypeQuery_MAX
};
enum EDrawDebugTrace
{
	None,
	ForOneFrame,
	ForDuration,
	Persistent

};
struct FHitResult
{

	char pad_88[0x88];


};
struct FLinearColor
{
	float	R,
		G,
		B,
		A;
	inline FLinearColor()
		: R(0), G(0), B(0), A(0)
	{ }

	inline FLinearColor(float r, float g, float b, float a)
		: R(r),
		G(g),
		B(b),
		A(a)
	{ }


};

struct BoneItem {
    int ID;
    int Flags;
    int Index;
};

struct BoneData {
    int Head; // 头部
    int Chest; // 胸部
    int Pelvis; // 盆骨
    int Hand_L; // 左手
    int Hand_R; // 右手
    int arm_lower_L; // 左胳膊
    int arm_lower_R; // 右胳膊
    int arm_upper_L; // 左肩膀
    int arm_upper_R; // 右肩膀
    int leg_upper_L; // 左腿
    int leg_upper_R; // 右腿
    int ankle_L; // 左脚
    int ankle_R; // 右脚
};


static double hd = 180 / M_PI;


static FRotator getPointingAngle(FVector camera, FVector xyz){
  FVector2D PointingAngle;
    //float ytime = distance / 88000;

 //   xyz.Z = xyz.Z + 360 * ytime * ytime;
    float zbcx = xyz.X - camera.X;
    float zbcy = xyz.Y - camera.Y ;
    float zbcz = xyz.Z - camera.Z ;
    float pfg =sqrt((zbcx * zbcx) + (zbcy*zbcy));
PointingAngle.Y= atan2(zbcz ,pfg) * hd ;

    
if(zbcx >= 1 && zbcy >= 1){
    PointingAngle.X = atan(zbcy / zbcx) * hd ;
     }
    if(zbcx <= 1 && zbcy >= 1){
    PointingAngle.X = atan(zbcy / zbcx) * hd +180 ;
    }
    if(zbcx <=1 && zbcy <= 1){
    PointingAngle.X = atan(zbcy / zbcx) * hd -180 ;
    }
    if(zbcx >= 1 && zbcy <= 1){
    PointingAngle.X = atan(zbcy / zbcx) * hd  ;
    }
    
    


    return FRotator(PointingAngle.Y, PointingAngle.X, 0.0f);

}

#include <sys/syscall.h>    // 读取数据
#include <sys/uio.h>	   	// 读取数据
#include <cmath> // 判断
#include <unistd.h>
#include <stdio.h>
#include <unordered_map>




namespace Data
{

    // symbols
    extern void* AHUD_ReceiveDrawHUD;
    extern void* AHUD_DrawText;
    extern void* UCanvas_K2_DrawLine;
    extern void* UUserWidget_AddToViewport;
    extern void* AHUD_GetTextSize;
    extern void* AGameMode_Say;
    
    extern uintptr_t Module;
    extern std::unordered_map < int, BoneData > BoneCache;

    std::string GetFromFName(const int index);

    bool isStartWith(std::string str, std::string check);
    bool isEqual(std::string s1, std::string s2);
    bool isContain(std::string str, std::string check);

   /**
        * 判断是否能读取
        * @param addr
        * @return
        */
    bool isCanRead(long addr);

	template<class T>
	T Ram(uintptr_t ptr)
	{
		T buff = {};
		if ((ptr > 0x1000000 - 0x8) && (ptr < 0x7FFFFFF00000 + 0x8)) {
			if (isCanRead(ptr)) {

				memmove(&buff, (void*)ptr, sizeof(T));
			}
		}
		return buff;
	}
	
	template<class T>
	void Write(uintptr_t ptr, T buf)
	{
		T buff = {};
		if ((ptr > 0x1000000 - 0x8) && (ptr < 0x7FFFFFF00000 + 0x8)) {
			if (isCanRead(ptr)) {

				memmove((void*)ptr, &buf, sizeof(T));
			}
		}
	}


    
    

    class UCanvas{
        public:
            inline void* K2_DrawLine(FVector2D start, FVector2D end, float Thickness, FLinearColor col){
                void*(*pf)(void*,FVector2D,FVector2D,float,FLinearColor);
                *(uintptr_t*) &pf = (uintptr_t)UCanvas_K2_DrawLine;
                return pf(this, start, end, Thickness, col);
            }
            inline void DrawCircle(float x, float y, float radius, int numsides, FLinearColor OutlineColor){
                float Step = M_PI * 2.0 / numsides;
            	int Count = 0;
            	FVector2D V[128];
            	for (float a = 0; a < M_PI * 2.0; a += Step)
            	{
            		float X1 = radius * cos(a) + x;
            		float Y1 = radius * sin(a) + y;
            		float X2 = radius * cos(a + Step) + x;
            		float Y2 = radius * sin(a + Step) + y;
            		V[Count].X = X1;
            		V[Count].Y = Y1;
            		V[Count + 1].X = X2;
            		V[Count + 1].Y = Y2;
            		K2_DrawLine(FVector2D(V[Count].X, V[Count].Y), FVector2D(X2, Y2), 1.f, OutlineColor);
            	}
            }
            inline void DrawRect(float x, float y, float w, float h, float Thickness, FLinearColor OutlineColor){
            	
                K2_DrawLine(FVector2D(x,y), FVector2D(x+w,y), 1.f, OutlineColor);
                K2_DrawLine(FVector2D(x,y+h), FVector2D(x+w,y+h), 1.f, OutlineColor);

                K2_DrawLine(FVector2D(x,y), FVector2D(x,y+h), 1.f, OutlineColor);
                K2_DrawLine(FVector2D(x+w,y), FVector2D(x+w,y+h), 1.f, OutlineColor);
            	
            }
    };

    class AHUD{
        public: 
            inline void* DrawText(FString Text, float X, float Y, FLinearColor col){
                void*(*pf)(void*,FString&,FLinearColor, float, float, void*, float, bool);
                *(uintptr_t*) &pf = (uintptr_t)AHUD_DrawText;
                return pf(this, Text, col, X, Y, NULL, 1.0f, false);
            }
            inline void* GetTextSize(FString Text, float& Width, float& Height, float Scale){
                void*(*pf)(void*,FString&, float&, float&, void*, float);
                *(uintptr_t*) &pf = (uintptr_t)AHUD_GetTextSize;
                return pf(this, Text, Width, Height, NULL, Scale);
            }
            
            

            char pad_270[0x270];
            UCanvas* Canvas;
    };
    
    class WeaponBase_C{
        public:
            char pad_3bc[0x3bc];
    	    int BulletNum;//[Offset: 0x3bc, Size: 0x4]
	        int KillMoney;//[Offset: 0x3c0, Size: 0x4]

    };
    
    
    

        
    class CSGOCharacterBase_C{
        public:
        
            inline FString GetCharacterName(){
                return CharacterName.Real->str;
            }
            
            inline BoneData GetBoneData(){
                BoneData data;
                auto Mesh = Ram<uintptr_t>(((uintptr_t)this)+ 0x280);
                if(!Mesh) return data;
                auto ModelID = Ram<int>(Mesh + 0x4f4);
                
                if(BoneCache.find(ModelID) == BoneCache.end())
                    return BoneCache[ModelID];
            
                auto StaticMesh = Ram<uintptr_t>(Mesh + 0x478);
                if(!StaticMesh) return data;
                struct Array{
                    uintptr_t Array;
                    int Count;
                };
                
                auto BoneArray = Ram<Array>(StaticMesh + 0x1B0);
                
                
                for(int i = 0; i < BoneArray.Count; i ++){
                    auto ItemAddr = BoneArray.Array + i * sizeof(BoneItem);
                    //LOGD("%p", ItemAddr);
                    auto Item = Ram<BoneItem>(ItemAddr);
                    
                    
                    //LOGD("Item");
                    auto BoneName = GetFromFName(Item.ID);
                    //LOGD("Name");
                    if(strstr(BoneName.c_str(), "head_0_")){
                        data.Head = Item.Index;
                    }else if(strstr(BoneName.c_str(), "neck")){
                        data.Chest = Item.Index;
                    }else if(strstr(BoneName.c_str(), "pelvis")){
                        data.Pelvis = Item.Index;
                    }else if(strstr(BoneName.c_str(), "hand_L")){
                        data.Hand_L = Item.Index;
                    }else if(strstr(BoneName.c_str(), "hand_R")){
                        data.Hand_R = Item.Index;
                    }else if(strstr(BoneName.c_str(), "arm_lower_L_")){
                        data.arm_lower_L = Item.Index;
                    }else if(strstr(BoneName.c_str(), "arm_lower_R_")){
                        data.arm_lower_R = Item.Index;
                    }else if(strstr(BoneName.c_str(), "arm_upper_L_")){
                        data.arm_upper_L = Item.Index;
                    }else if(strstr(BoneName.c_str(), "arm_upper_R_")){
                        data.arm_upper_R = Item.Index;
                    }else if(strstr(BoneName.c_str(), "leg_upper_L")){
                        data.leg_upper_L = Item.Index;
                    }else if(strstr(BoneName.c_str(), "leg_upper_R")){
                        data.leg_upper_R = Item.Index;
                    }else if(strstr(BoneName.c_str(), "ankle_L")){
                        data.ankle_L = Item.Index;
                    }else if(strstr(BoneName.c_str(), "ankle_R")){
                        data.ankle_R = Item.Index;
                    }
                    
                    //LOGD("Item: %p %d %d %s", ItemAddr, Item.Index, Item.ID, BoneName.c_str());
                }
                
                //LOGD("%p %d %d", BoneArray.Array, BoneArray.Count, ModelID);
                BoneCache[ModelID] = data;
                return data;
                
            }
            
            inline WeaponBase_C* GetCurrentWeapon(){
                switch(CurrentWeapon){
                    case 0:
                        return MainWeaponActor;
                        break;
                    case 1:
                        return PistolWeaponActor;
                        break;
                    case 2:
                        return KnifeWeaponActor;
                        break;
                }
                return NULL;
            }
        
        
            char pad_694[0x694];
            bool IsReloading;//(ByteOffset: 0, ByteMask: 1, FieldMask: 255)[Offset: 0x694, Size: 0x1]
            bool IsSwitching;//(ByteOffset: 0, ByteMask: 1, FieldMask: 255)[Offset: 0x695, Size: 0x1]
            WeaponBase_C* MainWeaponActor;//[Offset: 0x698, Size: 0x8]
        	WeaponBase_C* PistolWeaponActor;//[Offset: 0x6a0, Size: 0x8]
        	WeaponBase_C* KnifeWeaponActor;//[Offset: 0x6a8, Size: 0x8]
        	int CurrentWeapon;//[Offset: 0x6b0, Size: 0x4]
        	bool IsFiring;//(ByteOffset: 0, ByteMask: 1, FieldMask: 255)[Offset: 0x6b4, Size: 0x1]
        	char pad_6c4[0x6c4-0x6b4-0x1];
        	int RecoilCount;//[Offset: 0x6c4, Size: 0x4]
        	char pad_0x6d0[0x6d0-0x6c4-0x4];
        	float NextPitchOffset;//[Offset: 0x6d0, Size: 0x4]
        	float NextYawOffset;//[Offset: 0x6d4, Size: 0x4]
        	float MovementOffsetMax;//[Offset: 0x6d8, Size: 0x4]
        	float InputScaleShift;//[Offset: 0x6dc, Size: 0x4]
        	char pad_6e4[0x6e4-0x6dc-0x4];
        	int ReloadAmmoNeed;//[Offset: 0x6e4, Size: 0x4]
        	float CrosshairMovePitch;//[Offset: 0x6e8, Size: 0x4]
        	float CrosshairMoveYaw;//[Offset: 0x6ec, Size: 0x4]
            char pad_708[0x708-0x6ec-0x4];
            bool IsCT;//(ByteOffset: 0, ByteMask: 1, FieldMask: 255)[Offset: 0x708, Size: 0x1]
            char pad_784[0x784-0x708-0x1];
            float CHeal;//[Offset: 0x784, Size: 0x4]
        	float CHealMax;//[Offset: 0x788, Size: 0x4]
            char pad_7c8[0x7c8-0x788-0x4];
            bool IsDead;//(ByteOffset: 0, ByteMask: 1, FieldMask: 255)[Offset: 0x7c8, Size: 0x1]
        	char pad_830[0x830-0x7c8-0x1];
        	FText CharacterName;//[Offset: 0x830, Size: 0x18]
        	char pad_8a9[0x8a9-0x830-0x18];
	        bool IsFreeze;//[Offset: 0x8a9, Size: 0x1]

    };
    


 



    extern void* (*_Draw)(AHUD* hud, int Width, int Height);
	void* Draw(AHUD* hud, int Width,int Height);
	
	extern void* (*_Say)(void* GameMode, FString Text);
	void* Say(void* GameMode, FString Text);

	extern void (*_test_hook5)(void* obj, FVector& CamLoc, FRotator& CamRot);
    void test_hook5(void* obj, FVector& CamLoc, FRotator& CamRot);

	
}

