#include <iostream>
#include <Windows.h>
#include <thread>
#include <chrono>
#include <string>
#include <fstream>
#include <urlmon.h>
#include <nlohmann/json.hpp>

#pragma comment(lib, "Urlmon.lib")

using json = nlohmann::json;

const wchar_t* srcUrl = L"https://www.bing.com/HPImageArchive.aspx?format=js&idx=0&n=1&mkt=en-US";
std::string bingUrl = "https://www.bing.com";

const wchar_t* jsonFile = L"C:\\bDW.json";
const wchar_t* fileName = L"C:\\dWall.png";

int main()
{
    system("color 0E");
    system("title Daily Wallpapers");
    std::cout << "Starting app... This window will close in 5 seconds!\nYou can find it with Task Manager!\n";
    Sleep(5000);
    ::ShowWindow(::GetConsoleWindow(), SW_HIDE);

    do {
        try
        {
            if (S_OK == URLDownloadToFile(NULL, srcUrl, jsonFile, 0, NULL))
            {
                std::cout << "Saved to " << jsonFile << "\n\n";
            }
            else { std::cerr << "Failed to acquire file.\n"; }

            Sleep(250);

            std::ifstream f(jsonFile);
            json data = json::parse(f);

            std::string cleanedUp;
            cleanedUp = to_string(data["images"][0]["url"]);
            cleanedUp.erase(remove(cleanedUp.begin(), cleanedUp.end(), '\"'));
            bingUrl.append(cleanedUp);

            std::wstring stemp = std::wstring(bingUrl.begin(), bingUrl.end());
            LPCWSTR sw = stemp.c_str();

            if (S_OK == URLDownloadToFile(NULL, sw, fileName, 0, NULL))
                std::cout << "Saved file to " << fileName << "\n";
            else
                std::cerr << "Unable to acquire file.\n";

            Sleep(250);

            BOOL success = SystemParametersInfoW(SPI_SETDESKWALLPAPER, 0, (void*)fileName, SPIF_UPDATEINIFILE);

            if (!success)
            {
                std::cerr << GetLastError();
                return;
            }

            std::cout << "Successfully changed background!\nCleaning up..\n";
            std::remove("C:\\bDw.json");
            std::remove("C:\\dWall.png");
            std::cout << "Finished!\n";

            std::this_thread::sleep_for(std::chrono::hours(1));
        }
        catch (std::exception e)
        {
            std::cerr << e.what();
        }
    } while (true);
}