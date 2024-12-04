#pragma once

namespace pinguu {
	namespace sdk {
		namespace unreal {

			namespace defines {
				bool alternate_offsets = 0;
			}

			template<class k, class e>
			class tmap
			{
			public:
				k Key;
				e Element;
				char __pad0x[0x8];
			};

			template<class type> class array {
			public: friend class fstring;
			public:
				array() : data(nullptr), count(std::int32_t()), maxx(std::int32_t()) { }
				array(type* data, std::int32_t count, std::int32_t maxx) : data(data), count(count), maxx(maxx) { }

				const bool is_valid() const noexcept
				{
					return !(this->data == nullptr);
				}

				const std::int32_t size() const noexcept
				{
					return this->count;
				}

				type& operator[](std::int32_t index) noexcept
				{
					return this->data[index];
				}

				const type& operator[](std::int32_t index) const noexcept
				{
					return this->data[index];
				}

				bool is_valid_index(std::int32_t index) const noexcept
				{
					return index < this->size();
				}

			protected:
				type* data;
				std::int32_t count;
				std::int32_t maxx;
			};

			template<class type> class enum_as_byte {
			public:
				enum_as_byte() { }
				enum_as_byte(type value) : value(static_cast<std::uint8_t>(value)) { }

				explicit enum_as_byte(std::int32_t value) : value(static_cast<std::uint8_t>(value)) { }
				explicit enum_as_byte(std::uint8_t value) : value(value) { }

				operator type() const { return type(value); }
				type get() const { return type(value); }

			private:
				std::uint8_t value;
			};

			class fstring : public array<wchar_t> {
			public:
				fstring() {}
				fstring(const wchar_t* other)
				{
					if (this->maxx = this->count = *other ? static_cast<std::int32_t>(std::wcslen(other)) + 1 : 0)
						this->data = const_cast<wchar_t*>(other);
				};

				const wchar_t* c_str() const
				{
					return this->data;
				}
			};

			class ftextdata {
			public:
				char pad_0x0000[0x28];  //0x0000
				wchar_t* Name;          //0x0028 
				__int32 Length;         //0x0030 
			};

			struct ftext {
				ftextdata* Data;
				char UnknownData[0x10];

				wchar_t* Get() const {
					if (Data)
						return Data->Name;

					return nullptr;
				}
			};

			class fname {
			public:
				fname() : comparison_index(std::uint32_t()), number(std::int32_t()) {};
				fname(std::uint32_t index) : comparison_index(index), number(std::int32_t()) {};

				bool operator==(const fname& other) const noexcept {
					return this->comparison_index == other.comparison_index;
				};

			public:
				std::uint32_t comparison_index;
				std::int32_t number;
			};

			class fkey {
			public:
				fkey() : name() {}
				fkey(fname name) : name(name) {}

			public:
				fname name;
				std::uint8_t details[24] = {};
			};

			class frotator {
			public:
				frotator() : pitch(0.f), yaw(0.f), roll(0.f) {}
				frotator(float pitch, float yaw) : pitch(pitch), yaw(yaw), roll(0.f) {}
				frotator(float pitch, float yaw, float roll) : pitch(pitch), yaw(yaw), roll(roll) {}

				frotator operator + (const frotator& other) const { return { this->pitch + other.pitch, this->yaw + other.yaw, this->roll + other.roll }; }
				frotator operator - (const frotator& other) const { return { this->pitch - other.pitch, this->yaw - other.yaw, this->roll - other.roll }; }
				frotator operator * (float scalar) const { return { this->pitch * scalar, this->yaw * scalar, this->roll * scalar }; }
				frotator operator / (float divide) const { return { this->pitch / divide, this->yaw / divide, this->roll / divide }; }

				frotator& operator *= (const float other) { this->yaw *= other; this->pitch *= other; this->roll *= other; return *this; }
				frotator& operator /= (const float other) { this->yaw /= other; this->pitch /= other; this->roll /= other; return *this; }

				frotator& operator =  (const frotator& other) { this->pitch = other.pitch; this->yaw = other.yaw; this->roll = other.roll; return *this; }
				frotator& operator += (const frotator& other) { this->pitch += other.pitch; this->yaw += other.yaw; this->roll += other.roll; return *this; }
				frotator& operator -= (const frotator& other) { this->pitch -= other.pitch; this->yaw -= other.yaw; this->roll -= other.roll; return *this; }
				frotator& operator *= (const frotator& other) { this->pitch *= other.pitch; this->yaw *= other.yaw; this->roll *= other.roll; return *this; }
				frotator& operator /= (const frotator& other) { this->pitch /= other.pitch; this->yaw /= other.yaw; this->roll /= other.roll; return *this; }

				friend bool operator==(const frotator& first, const frotator& second)
				{
					return first.pitch == second.pitch && first.yaw == second.yaw && first.roll == second.roll;
				}

				friend bool operator!=(const frotator& first, const frotator& second)
				{
					return !(first == second);
				}

			public:
				float pitch = 0.f;
				float yaw = 0.f;
				float roll = 0.f;
			};

			class fvector2d {
			public:
				fvector2d() : x(0.f), y(0.f) {}
				fvector2d(float x, float y) : x(x), y(y) {}

				fvector2d operator + (const fvector2d& other) const { return { this->x + other.x, this->y + other.y }; }
				fvector2d operator - (const fvector2d& other) const { return { this->x - other.x, this->y - other.y }; }
				fvector2d operator * (float scalar) const { return { this->x * scalar, this->y * scalar }; }
				fvector2d operator / (float divide) const { return { this->x / divide, this->y / divide }; }

				fvector2d& operator *= (const float other) { this->x *= other; this->y *= other; return *this; }
				fvector2d& operator /= (const float other) { this->x /= other; this->y /= other; return *this; }

				fvector2d& operator =  (const fvector2d& other) { this->x = other.x; this->y = other.y; return *this; }
				fvector2d& operator += (const fvector2d& other) { this->x += other.x; this->y += other.y; return *this; }
				fvector2d& operator -= (const fvector2d& other) { this->x -= other.x; this->y -= other.y; return *this; }
				fvector2d& operator *= (const fvector2d& other) { this->x *= other.x; this->y *= other.y; return *this; }
				fvector2d& operator /= (const fvector2d& other) { this->x /= other.x; this->y /= other.y; return *this; }

				friend bool operator==(const fvector2d& first, const fvector2d& second)
				{
					return first.x == second.x && first.y == second.y;
				}

				friend bool operator!=(const fvector2d& first, const fvector2d& second)
				{
					return !(first == second);
				}

			public:
				float x = 0.f;
				float y = 0.f;
			};

			class fvector {
			public:
				fvector() : x(0.f), y(0.f), z(0.f) {}
				fvector(float x, float y, float z) : x(x), y(y), z(z) {}
				fvector(const frotator& rotator) : x(rotator.pitch), y(rotator.yaw), z(rotator.roll) {}

				fvector operator + (const fvector& other) const { return { this->x + other.x, this->y + other.y, this->z + other.z }; }
				fvector operator - (const fvector& other) const { return { this->x - other.x, this->y - other.y, this->z - other.z }; }
				fvector operator * (float scalar) const { return { this->x * scalar, this->y * scalar, this->z * scalar }; }
				fvector operator / (float divide) const { return { this->x / divide, this->y / divide, this->z / divide }; }

				fvector& operator *= (const float other) { this->x *= other; this->y *= other; this->z *= other; return *this; }
				fvector& operator /= (const float other) { this->x /= other; this->y /= other; this->z /= other; return *this; }

				fvector& operator =  (const fvector& other) { this->x = other.x; this->y = other.y; this->z = other.z; return *this; }
				fvector& operator += (const fvector& other) { this->x += other.x; this->y += other.y; this->z += other.z; return *this; }
				fvector& operator -= (const fvector& other) { this->x -= other.x; this->y -= other.y; this->z -= other.z; return *this; }
				fvector& operator *= (const fvector& other) { this->x *= other.x; this->y *= other.y; this->z *= other.z; return *this; }
				fvector& operator /= (const fvector& other) { this->x /= other.x; this->y /= other.y; this->z /= other.z; return *this; }

				float size() {
					return sqrt(x * x + y * y + z * z);
				}

				fvector& clamp() {
					if (this->x < -89.f) this->x += 360.f;
					else if (this->x > 89.f) this->x -= 360.f;

					while (this->y < -180.f) this->y += 360.f;
					while (this->y > 180.f) this->y -= 360.f;
					this->z = 0.f;

					return *this;
				}

				friend bool operator==(const fvector& first, const fvector& second)
				{
					return first.x == second.x && first.y == second.y && first.z == second.z;
				}

				friend bool operator!=(const fvector& first, const fvector& second)
				{
					return !(first == second);
				}

			public:
				float x = 0.f;
				float y = 0.f;
				float z = 0.f;
			};

			class flinearcolor {
			public:
				flinearcolor() : red(0.f), green(0.f), blue(0.f), alpha(0.f) {}
				flinearcolor(float red, float green, float blue, float alpha) : red(red), green(green), blue(blue), alpha(alpha) {}

			public:
				float red;
				float green;
				float blue;
				float alpha;

				flinearcolor real() {
					return { this->red / 255, this->green / 255, this->blue / 255, this->alpha / 255 };
				}
			};

			struct FQuat
			{
				float x, y, z, w;
			};

			struct FTransform
			{
				FQuat rotation;				
				fvector scale3d;
				fvector translation;		
			};

			//struct fhitresult {
			//	char bBlockingHit : 1; // 0x00(0x01)
			//	char bStartPenetrating : 1; // 0x00(0x01)
			//	char pad_0_2 : 6; // 0x00(0x01)
			//	char pad_1[0x3]; // 0x01(0x03)
			//	int32_t FaceIndex; // 0x04(0x04)
			//	float Time; // 0x08(0x04)
			//	float Distance; // 0x0c(0x04)
			//	fvector Location; // 0x10(0x0c)
			//	fvector ImpactPoint; // 0x1c(0x0c)
			//	fvector Normal; // 0x28(0x0c)
			//	fvector ImpactNormal; // 0x34(0x0c)
			//	fvector TraceStart; // 0x40(0x0c)
			//	fvector TraceEnd; // 0x4c(0x0c)
			//	float PenetrationDepth; // 0x58(0x04)
			//	int32_t Item; // 0x5c(0x04)
			//	char ElementIndex; // 0x60(0x01)
			//	char pad_61[0x3]; // 0x61(0x03)
			//	void* PhysMaterial; // 0x64(0x08)
			//	void* Actor; // 0x6c(0x08)
			//	void* Component; // 0x74(0x08)
			//	fname BoneName; // 0x7c(0x0c)
			//	fname MyBoneName; // 0x88(0x0c)
			//	char pad_88[0x60]; // 0x61(0x03)
			//};

			struct fhitresult {
				char bBlockingHit : 1; // 0x00(0x01)
				char bStartPenetrating : 1; // 0x00(0x01)
				char pad_0_2 : 6; // 0x00(0x01)
				char pad_1[0x3]; // 0x01(0x03)
				int32_t FaceIndex; // 0x04(0x04)
				float Time; // 0x08(0x04)
				float Distance; // 0x0c(0x04)
				fvector Location; // 0x10(0x0c)
				fvector ImpactPoint; // 0x1c(0x0c)
				fvector Normal; // 0x28(0x0c)
				fvector ImpactNormal; // 0x34(0x0c)
				fvector TraceStart; // 0x40(0x0c)
				fvector TraceEnd; // 0x4c(0x0c)
				float PenetrationDepth; // 0x58(0x04)
				int32_t Item; // 0x5c(0x04)
				char ElementIndex; // 0x60(0x01)
				char pad_61[0x3]; // 0x61(0x03)
				char pad_64[0x8]; // 0x64(0x08)
				char pad_65[0x8];
				char pad_66[0x8];
				fname BoneName; // 0x7c(0x0c)
				fname MyBoneName; // 0x88(0x0c)
			};

			class fcolor {
			public:
				fcolor() : red(0), green(0), blue(0), alpha(0) {}
				fcolor(int red, int green, int blue, int alpha) : red(red), green(green), blue(blue), alpha(alpha) {}

			public:
				int red;
				int green;
				int blue;
				int alpha;
			};

			class fplane : public fvector {
			public:
				fplane() : w(0.f) {}

			public:
				float w;
			};

			class fmatrix {
			public:
				fmatrix() : x(fplane()), y(fplane()), z(fplane()), w(fplane()) {}
			public:
				fplane x;
				fplane y;
				fplane z;
				fplane w;
			};

			class uclass;
			class uobject {
			public:
				void** vtable;
				std::int32_t flags;
				std::int32_t index;
				uclass* class_private;
				fname name;
				uobject* outer;

				void process_event(uobject* function, void* args, void* out_args = nullptr, void* stack = nullptr) {;
					if (!function || !this)
						return;

					static void(__fastcall * process_event_func)(uobject*, uobject*, void*, void*, void*);
					if (!process_event_func) {
						process_event_func = reinterpret_cast<decltype(process_event_func)>(memory::get_base_address() + DecryptOffsetClear(offsets::main::process_event));
					}
					return spoof_call(process_event_func)(this, function, args, out_args, stack);
				}

				template<typename type = uobject> static inline type* find_object(const wchar_t* name, uobject* outer = nullptr, bool exact = false) {
					return reinterpret_cast<type*>(uobject::static_find_object(nullptr, outer, name, exact));
				}

				template<typename type = uobject> static inline type* load_object(const wchar_t* name, uobject* outer = nullptr) {
					return reinterpret_cast<type*>(uobject::static_load_object(nullptr, outer, name, 0, 0, 0, true, 0));
				}

				static inline void* static_find_object(uclass* klass, uobject* outer, const wchar_t* name, bool exact) {
					static void*(__fastcall * find_obj_func)(uclass*, uobject*, const wchar_t*, bool);
					if (!find_obj_func) {
						find_obj_func = reinterpret_cast<decltype(find_obj_func)>(memory::get_base_address() + DecryptOffsetClear(offsets::main::static_find_object));
					}
					return spoof_call(find_obj_func)(klass, outer, name, exact);
				}

				/*void mark_render_state_dirty() {
					static void*(__fastcall * mark_render_state_dirty_func)(uobject*);
					if (!mark_render_state_dirty_func) {
						mark_render_state_dirty_func = reinterpret_cast<decltype(mark_render_state_dirty_func)>(base + DecryptOffsetClear(offsets::main::render_dirty));
					}

					spoof_call(mark_render_state_dirty_func)(this);
				}*/

				/*static inline void* static_load_object(uobject* obj_class, uobject* outer, const wchar_t* origInName, const wchar_t* filename, UINT32 load_flags, uintptr_t sandbox, bool allow_recon, uintptr_t instancing_context) {
					static void* (__fastcall* load_obj_func)(uobject*, uobject*, const wchar_t*, const wchar_t*, UINT32, uintptr_t, bool, uintptr_t);
					if (!load_obj_func) {
						load_obj_func = reinterpret_cast<decltype(load_obj_func)>(base + DecryptOffsetClear(offsets::main::static_load_object));
					}
					return spoof_call(load_obj_func)(obj_class, outer, origInName, filename, load_flags, sandbox, allow_recon, instancing_context);
				}*/

				fstring get_name() {		
					static uobject* system;
					if (!system) {
						auto enc = Encrypt(L"Engine.Default__KismetSystemLibrary");
						system = this->find_object(enc.decrypt()), enc.clear();
					}

					static uobject* function;
					if (!function) {
						auto enc = Encrypt(L"Engine.KismetSystemLibrary.GetObjectName");
						function = this->find_object(enc.decrypt()), enc.clear();
					}

					struct {
						uobject* object;
						fstring return_value;
					} params = { this };

					this->process_event(function, &params);

					return params.return_value;
				}
			};

			class ufield : public uobject
			{
			public:
				ufield* next;
			};

			class ustruct : public ufield
			{
			public:
				ustruct* super_field;
				ufield* children;
				std::int32_t property_size;
				std::int32_t min_alignment;
			};

			class uclass : public ustruct
			{

			};

		}
	}
}

using namespace pinguu::sdk::unreal;