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

#define min(a, b) a < b ? a : b;
#define max(a, b) a > b ? a : b;

	virtual bounds_t getBounds() {
		bounds_t bounds_tmp = Object::getBounds();
 
 		/* go through all group members, find the min/max of each ordinal */
		std::vector< Object* >::iterator itr = list.begin();
		while(itr != list.end()) {
			bounds_t b = (*itr)->getBounds();
			bounds_tmp.min.x = min(bounds_tmp.min.x, b.min.x);
			bounds_tmp.min.y = min(bounds_tmp.min.y, b.min.y);
			bounds_tmp.min.z = min(bounds_tmp.min.z, b.min.z);
			bounds_tmp.max.x = max(bounds_tmp.max.x, b.max.x);
			bounds_tmp.max.y = max(bounds_tmp.max.y, b.max.y);
			bounds_tmp.max.z = max(bounds_tmp.max.z, b.max.z);
			itr++;
		}

		/* do any group transforms there are on the final min/max */
		glPushMatrix();
			glLoadIdentity();
			glTranslatef( xlate[0], xlate[1], xlate[2] ); 
			glMultMatrixf(rotate.array());
			glScalef(scale[0], scale[1], scale[2]);
			
			GLfloat minf[] = {1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  bounds_tmp.min.x, bounds_tmp.min.y, bounds_tmp.min.z, 1};
			glMultMatrixf(minf);
			
			glGetFloatv(GL_MODELVIEW_MATRIX, minf);
			bounds_tmp.min.x = minf[12];
			bounds_tmp.min.y = minf[13];
			bounds_tmp.min.z = minf[14];

			glLoadIdentity();
			glTranslatef( xlate[0], xlate[1], xlate[2] ); 
			glMultMatrixf(rotate.array());
			glScalef(scale[0], scale[1], scale[2]);
			
			GLfloat maxf[] = {1, 0, 0, 0,  0, 1, 0, 0,  0, 0, 1, 0,  bounds_tmp.max.x, bounds_tmp.max.y, bounds_tmp.max.z, 1};
			glMultMatrixf(maxf);
			
			glGetFloatv(GL_MODELVIEW_MATRIX, maxf);
			bounds_tmp.max.x = maxf[12];
			bounds_tmp.max.y = maxf[13];
			bounds_tmp.max.z = maxf[14];
		glPopMatrix();

		return bounds_tmp;
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

   virtual void printSceneGraph(int *tab) {
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
