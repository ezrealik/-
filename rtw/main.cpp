#include "../Left4Dead2Plugins/json/json.h"
#include <Windows.h>
#pragma warning(disable:4996)
int main() {
	char str[] = "{\"uploadid\": \"UP000000\",\"code\": 100,\"msg\": \"\",\"files\": \"\"}";
	FILE *fp;
	fp = fopen("E:\\Project Code\\C&C++\\Other\\Left4Dead2Plugins\\Debug\\Mods\\left4dead2\\addons\\sourcemod\\configs\\core.cfg", "rb");
	char br[8068] = { 0 };
	fread(br, 8068, 1, fp);
	Json::Reader reader;
	Json::Value root;
	std::string code;
	if (!root["Core"].isNull())  // 访问节点，Access an object value by name, create a null member if it does not exist.  
		code = root["Core"].asString();
	if (reader.parse(br, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
	{
		std::string upload_id = root["Core"].asString();  // 访问节点，upload_id = "UP000000"  
		std::string code = root["PassInfoVar"].asString();    // 访问节点，code = 100 
	}
	return 0;
}
