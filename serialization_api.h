#ifndef ____applianceState__
#define ____applianceState__

#include <iostream>
#include <exception>
#include <boost/property_tree/ptree.hpp>
#include <boost/optional.hpp>
using boost::property_tree::ptree;
using namespace std;

class Colors{
	public : 
		Colors();
		Colors(uint64_t hue,uint8_t sat,uint16_t val);
		ptree serializeColor();
	protected :
		uint64_t h;
		uint8_t  s;
		uint16_t v;
};

class ApplianceState {
	public : 
		ApplianceState();
		ApplianceState(bool isReachable);
		
		void set_isActive(bool value);
		void set_dimming(int value);
		void set_isReachable(bool value);
		void set_energy(int value);
		void set_colors(uint64_t hue,uint8_t sat,uint16_t val);
		string serialize(string id);
		ptree serializeSubtree(string id);
	protected :
		optional<bool> isActive;
		optional<int> dimming;
		optional<bool> isReachable;
		optional<int> energy;
		Colors *color;
};

Colors :: Colors(){
	this->h = -1;
	this->s = -1;
	this->v = -1;
}
Colors :: Colors(uint64_t hue,uint8_t sat,uint16_t val){
	this->h = hue;
	this->s = sat;
	this->v = val;
}

ApplianceState :: ApplianceState(){
	color = NULL;
}

ApplianceState :: ApplianceState(bool isReachable){
	this->isReachable = isReachable;
	color = NULL;
}

void ApplianceState :: set_isActive(bool value){
	this->isActive = value;
}

void ApplianceState :: set_dimming(int value){
	this->dimming = value;
}

void ApplianceState :: set_isReachable(bool value){
	this->isReachable = value;
}

void ApplianceState :: set_energy(int value){
	this->energy = value;
}

void ApplianceState :: set_colors(uint64_t hue,uint8_t sat,uint16_t val){
	color = new Colors(hue,sat,val);
}

ptree  Colors :: serializeColor(){
	ptree child4;
	child4.put("h",this->h);
	child4.put("s",this->s);
	child4.put("v",this->v);
	return child4;
}

ptree ApplianceState :: serializeSubtree(string id){
	ptree child1,child2,child3;
	bool  isExistChild3 = false , isExistChild2 = false ;
	try{
		// checking existence of child3 then adding key,value to child3
		if(this->energy.is_initialized()){
			child3.put("energy",this->energy);
			isExistChild3 = true;
		}
		if(color){
			child3.add_child("color",color->serializeColor());
			isExistChild3 = true;
		}
		
		// checking existence of child2  then adding key,value to child2
		if(this->isActive.is_initialized()){
			(this->isActive==true) ? child2.put("isActive","true") : child2.put("isActive","false");
			isExistChild2 = true;
		}
		if(this->dimming.is_initialized()){
			child2.put("dimming",this->dimming);
			isExistChild2 = true;
		}
		if(this->isReachable.is_initialized()){
			(this->isReachable==true) ?	child2.put("isReachable","true") : child2.put("isReachable","false");
			isExistChild2 = true;
		}
		
		// adding child3 to child2 if child3 exists  
		if(isExistChild3) child2.add_child("params",child3);
		
		// child1 will exists as id need not to be check for existence
		child1.put("id",id);
		if(isExistChild2) child1.add_child("state",child2);
		
	}catch(std::exception e){
		cout <<e.what()<<endl;
	}
	
	return child1;
}

string ApplianceState :: serialize(string id){
		ptree pt;
		ptree children;
		stringstream json;
		children.push_back(std::make_pair("", serializeSubtree(id)));
		pt.add_child("states", children);
		write_json(json,pt,false);
		return json.str();
}


#endif
/* Reference
 http://stackoverflow.com/questions/2114466/creating-json-arrays-in-boost-using-property-trees
 http://stackoverflow.com/questions/3751357/c-how-to-create-an-array-using-boostproperty-tree

 Note : to change the format of json output according to its data type i.e {"key":"1"} to {"key":1} please refer below link ...
 https://svn.boost.org/trac/boost/attachment/ticket/9496/boost_property_tree_json.patch
*/
