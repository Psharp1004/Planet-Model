#include "vector3f.h"
class AdvancedCamera{
	public:
		// Main camera orientation vectors
		Vector3f up;
		Vector3f right;
		Vector3f forward;
		Vector3f position;
		Vector3f rotation;
		Vector3f lookat;

		float maxYAngle;
		float camSpeed;
		int currentCursorMode;
		float mouseSensitivity;
		bool restrainY;

		float viewport_width;
		float viewport_height;
		float near_plane;
		float far_plane;
		float view_angle;

		// Constructor and destructor methods
		AdvancedCamera(Vector3f, float, float);
		~AdvancedCamera();

		// Movement and rotation methods
		void rotateY(float);
		void rotateX(float);
		void move(float);
		void strafe(float);
		void updateLook();
		void rotateWithMouse(int, int);
		void setMouseSensitivity(float);
		void reshapeFrustum(int, int, float, float, float);
};