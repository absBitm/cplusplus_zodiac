#include <iostream>
#include <string>
#include <curl/curl.h> //your directory may be different
using namespace std;

string data; //will hold the url's contents

size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up){
    for (int c = 0; c<size*nmemb; c++){
        data.push_back(buf[c]);
    }
    return size*nmemb; //tell curl how many bytes we handled
}

int main(){
    CURL* curl; //our curl object

    curl_global_init(CURL_GLOBAL_ALL); //pretty obvious
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, "http://stackoverflow.com/questions/10077745/twistedweb-on-multicore-multiprocessor");
    /*
    you can add post fields also to this curl url ...
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "command=O&parameter=1");
    */
    curl_easy_perform(curl);

    cout << endl << data << endl;
    curl_easy_cleanup(curl);
    curl_global_cleanup();
    return 0;
}


/* 
   compile & run 
   g++ -o curlRequest curlRequest.cpp -lcurl
   ./curlRequest
*/
