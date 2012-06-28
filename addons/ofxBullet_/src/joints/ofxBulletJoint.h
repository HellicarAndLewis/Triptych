/*
 *  ofxBulletJoint.h
 *  ofxBullet_v7_joints
 *
 *  Created by Nick Hardeman on 7/6/11.
 *  Copyright 2011 Arnold Worldwide. All rights reserved.
 *
 */

#pragma once
#include "ofMain.h"
#include "btBulletDynamicsCommon.h"
#include "ofxBulletConstants.h"
#include "ofxBulletUtils.h"
#include "ofxBulletBaseShape.h"

// creates a btGeneric6DofConstraint joint, free rotation, no constraints //
class ofxBulletJoint {
public:
	ofxBulletJoint();
	virtual ~ofxBulletJoint();
	
	virtual void	create( btDiscreteDynamicsWorld* a_world, ofxBulletBaseShape* a_shape1, ofxBulletBaseShape* a_shape2 );
	virtual void	create( btDiscreteDynamicsWorld* a_world, ofxBulletBaseShape* a_shape, ofVec3f a_pos );
	
	/******************************/
	// call before calling add() //
	virtual void	setLinearLowerLimit( ofVec3f a_limit );
	virtual void	setLinearLowerLimit( float a_x, float a_y, float a_z );
	virtual void	setLinearUpperLimit( ofVec3f a_limit );
	virtual void	setLinearUpperLimit( float a_x, float a_y, float a_z );
	virtual void	setAngularLowerLimit( ofVec3f a_limit );
	virtual void	setAngularLowerLimit( float a_x, float a_y, float a_z );
	virtual void	setAngularUpperLimit( ofVec3f a_limit );
	virtual void	setAngularUpperLimit( float a_x, float a_y, float a_z );
	/******************************/
	
	virtual void	add();
	
	virtual ofVec3f getPivotAWorldPos();
	virtual ofVec3f getPivotBWorldPos();
	
	virtual btRigidBody* getRigidBodyA() const;
	virtual btRigidBody* getRigidBodyB() const;
	virtual ofVec3f getPositionA() const;
	virtual ofVec3f getPositionB() const;
	
	virtual void	updatePivotPos( const ofVec3f a_pos, float a_length );
	
	virtual void	draw();
	virtual void	drawJointConstraints();
	
	virtual void	remove();
	
	void setParam(int param, float value);
	
protected:
	void _setDefaults();
	btDiscreteDynamicsWorld*	_world;	
private:

	btGeneric6DofConstraint*	_joint;
	ofVec3f						_targetPos;
	
protected:
	// is there two bodies the joint is connecting? if not, what is the target pos //
	bool						_bTwoBodies;
	bool						_bCreated;
	bool						_bAdded;
};

