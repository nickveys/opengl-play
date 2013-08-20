#ifndef GROUP_H_
#define GROUP_H_

#include <vector>

/*
 * An Object built to aggregate other objects
 */
class Group : public Object {
	public:

	virtual ~Group() {
		// delete all contained objects
		std::vector< Object* >::iterator itr = list.begin();
		while(itr != list.end()) {
			Object* o = *itr;
			itr = list.erase(itr);
			delete o;
		}
	}
  
	// add an object to the group
	void addObject(Object *o) {
		// make sure we don't jump into a different origin (if adding to a reopened group)
		for(int i = 0; i < 3; i++) {
			o->xlate[i] -= xlate[i];
		}
		list.push_back(o);
	}

	// remove an object from the group
	void removeObject(Object *o) {
		std::vector< Object* >::iterator iter = std::find(list.begin(), list.end(), o);
		list.erase(iter);
	}

	virtual void setSelected(bool s) {
		Object::setSelected(s);

		// let contained object know they are selected as well
		std::vector< Object* >::const_iterator itr = list.begin();
		while(itr != list.end()) {
			(*itr)->setSelected(s);
			itr++;
		}
	}

   virtual void printSceneGraph(int *tab)
   {
		(*tab)++;
		for(int i = 0; i < (*tab); i++) cout << "\t";
		cout << "Group" << endl;
		std::vector< Object* >::const_iterator itr = list.begin();
		while(itr != list.end()) {
			(*itr)->printSceneGraph(tab);
			itr++;
		}
		(*tab)--;
   }

protected:

	void typeSpecificDraw() {
		// draw all members of the group
		std::vector< Object* >::const_iterator itr = list.begin();
		while(itr != list.end()) {
			(*itr)->draw();
			itr++;
		}
	}

	// the list of objects.
	std::vector<Object*> list;  
};

#endif /*GROUP_H_*/
