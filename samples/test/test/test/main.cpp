//
//  main.cpp
//  test
//
//  Created by Xuhui on 16/7/14.
//  Copyright © 2016年 Xuhui. All rights reserved.
//

#include "net/URLRequest.h"
#include "net/HTTPSession.h"
#include "net/HTTPClient.h"
#include "net/URLResponse.h"
#include "base/IOBuffer.h"
#include "base/message_loop/MessageLoop.h"
#include <iostream>
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

ofstream out("test.txt");

int main(int argc, const char * argv[]) {
    // insert code here...
    shared_ptr<WukongBase::Net::URLRequest> request(new WukongBase::Net::URLRequest("http://www.163.com"));
    request->setHTTPMethod("GET");
    WukongBase::Net::HTTPClient client(4);
    

    out.close();
    
    WukongBase::Base::MessageLoop loop;
    loop.attachToCurrentThread();
    loop.postTask([&](){
        client.setUserAgent("Mozilla/5.0 (iPhone; U; CPU like Mac OS X; en) AppleWebKit/420+ (KHTML, like Gecko) Version/3.0 Mobile/1A543 Safari/419.3");
        client.executeRequest(request, [](const WukongBase::Net::HTTPSession&, const shared_ptr<WukongBase::Net::URLRequest>&, bool){
            
        }, [](const WukongBase::Net::HTTPSession&, const shared_ptr<WukongBase::Net::URLRequest>&, WukongBase::Net::URLResponse&& response) {
            const unordered_map<std::string, std::string>& headers = response.getAllHeader();
            for (unordered_map<std::string, std::string>::const_iterator iter = headers.begin(); iter != headers.end(); ++iter) {
                cout << iter->first << ":" << iter->second << endl;
            }
        }, [](const WukongBase::Net::HTTPSession&, const shared_ptr<WukongBase::Net::URLRequest>&, WukongBase::Base::IOBuffer&& buffer) {
            std::string str(buffer.data(), buffer.size());
            cout << str;
        }, [](const WukongBase::Net::HTTPSession&, const shared_ptr<WukongBase::Net::URLRequest>&) {
        });
    });
    loop.run();
    return 0;
}
