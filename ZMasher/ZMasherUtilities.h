#pragma once

#define ZERO_MEMORY(var) ZeroMemory(&var, sizeof(decltype(var)))


namespace ZMasher
{
	float GetRadians(float degrees);
	float GetDegrees(float radians);
	
}



/* MOCKUP FOR AN API USECASE


////////////////////////////// MODELS
	ZMModel* model = ZMasher::Instance()->GetModel("model.mesh");	//this function loads the model(if not already loaded) and returns an
																	//instance of the model to the user. It defaults to a shader
																	//based on the vertextype the model has, but can be overriden.
																	//should/can the shader be overriden runtime without the need to reload?



	//every update

	model->SetOrientation(&m_orientation); //overloaded function that can take any datatype, or simpy a MZMatrix44<float> or something :p
	model->Update(frameTime); //necessary? eventually for animations, not sure if necessary for static models

///////////////////////////// RENDERER
	ZMasherMain::Instance()->Init(args); //should be able to set things like windowsize, fullscreen, vsync, framerate, titlebarname, frameCapping etc.

	ZMasherMain::Instance()->Update();	//returns false if prompted to shut down(?).
										//should be called each frame, create a threaded environment for this?
										//
	
	ZMasherMain::Instance()->Resize(size);


*/