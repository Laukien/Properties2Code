/*  Properties2Code v.0.2.0 by Stephan Laukien */

#ifndef TEST_H
#define TEST_H

class test {
	private:
		std::string key;
		int integer;
		bool boolean;
		std::string main_sub;
	public:
		test() {
			key = "value",
			integer = 123,
			boolean = true,
			main_sub = "example";
		}
		void clean();
		void show();
		void load(const std::string &filename);
		void save(const std::string &filename);
		void open(const std::string &filename);
		void edit();

		void setKey(const std::string &value);
		std::string getKey();
		void setInteger(const int value);
		int getInteger();
		void setBoolean(const bool value);
		bool getBoolean();
		void setMainSub(const std::string &value);
		std::string getMainSub();
};

#endif
