//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Parent.h"
#include "MyMessage_m.h"

Define_Module(Parent);

void Parent::initialize()
{
    // TODO - Generated method body
    // choose pairs to talk randomly every 30 seconds with a probability of 50% that no pairs start talking at all at this time step.
    double interval = exponential(1 / par("lambda").doubleValue());
    scheduleAt(simTime() + interval, new cMessage(""));

}

void Parent::handleMessage(cMessage *msg)
{
    // TODO - Generated method body
    int i=1;
      while(true){
          int rand=uniform(0,1)*10;
        //  if(rand<5){
              int node1=uniform(0,1)*par("n").intValue();
              int node2;
              do{
              node2=uniform(0,1)*par("n").intValue();
              }while(node2==node1);
              MyMessage_Base* mmsg;
              if(node1>node2)mmsg=new MyMessage_Base(std::to_string(node1-1).c_str(),2);
              else mmsg=new MyMessage_Base(std::to_string(node1).c_str(),2);
              mmsg->setM_Type(2);
             // sleep(10);
              EV<<mmsg->getM_Type();
              mmsg->setM_Payload(("file"+std::to_string(i++)).c_str());
              send(mmsg,"outs",node2);
              MyMessage_Base* mmmsg;
              if(node2>node1) mmmsg=new MyMessage_Base(std::to_string(node2-1).c_str(),2);
              else mmmsg=new MyMessage_Base(std::to_string(node2).c_str(),2);
               mmmsg->setM_Type(2);
              mmmsg->setM_Payload(("file"+std::to_string(i++)).c_str());
              send(mmmsg,"outs",node1);
              if(i>2)break;
             // mmsg->setM_Type(2);
              //mmsg->setName(s);
              //dest=atoi(mmsg->getName());
              //fileName=mmsg->getM_Payload();
          //}
          sleep(30);
       }
}
