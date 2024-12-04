namespace pinguu
{
	namespace keys
	{
		static fname left_mouse_button;
		static fname right_mouse_button;
		static fname thumb_mouse_button;
		static fname thumb_mouse_button2;
		static fname left_alt;
		static fname left_shift;
		static fname space_bar;
		static fname v;
		static fname insert;

		static fname convert_key(int key_code)
		{
			if (key_code == 0) return left_mouse_button;
			else if (key_code == 1) return right_mouse_button;
			else if (key_code == 2) return thumb_mouse_button;
			else if (key_code == 3) return thumb_mouse_button2;
			else if (key_code == 4) return left_alt;
			else if (key_code == 5) return left_shift;
			else if (key_code == 6) return v;

			return left_mouse_button;
		}

		void run()
		{
			auto enc_1 = Encrypt(L"LeftMouseButton");
			auto enc_2 = Encrypt(L"RightMouseButton");
			auto enc_3 = Encrypt(L"ThumbMouseButton");
			auto enc_4 = Encrypt(L"ThumbMouseButton2");
			auto enc_5 = Encrypt(L"LeftAlt");
			auto enc_6 = Encrypt(L"LeftShift");
			auto enc_7 = Encrypt(L"SpaceBar");
			auto enc_8 = Encrypt(L"V");
			auto enc_9 = Encrypt(L"Insert");

			left_mouse_button = string::string_to_name(enc_1.decrypt()), enc_1.encrypt();
			right_mouse_button = string::string_to_name(enc_2.decrypt()), enc_2.encrypt();
			thumb_mouse_button = string::string_to_name(enc_3.decrypt()), enc_3.encrypt();
			thumb_mouse_button2 = string::string_to_name(enc_4.decrypt()), enc_4.encrypt();
			left_alt = string::string_to_name(enc_5.decrypt()), enc_5.encrypt();
			left_shift = string::string_to_name(enc_6.decrypt()), enc_6.encrypt();
			space_bar = string::string_to_name(enc_7.decrypt()), enc_7.encrypt();
			v = string::string_to_name(enc_8.decrypt()), enc_8.encrypt();
			insert = string::string_to_name(enc_9.decrypt()), enc_9.encrypt();

			printf("keys run\n");
		}
	}
}