#pragma once

#include <Box2D/Box2D.h>
#include <v8pp/module.hpp>
#include <v8pp/class.hpp>
#include <v8.h>

inline v8pp::module getB2Mod(v8::Isolate* isolate)
{
	v8pp::module b2mod(isolate);

	v8pp::class_<b2BlockAllocator> blockAllocatorClass(isolate);
	blockAllocatorClass.ctor<>();
	blockAllocatorClass.auto_wrap_objects();
	//blockAllocatorClass.function("Allocate", &b2BlockAllocator::Allocate);
	//blockAllocatorClass.function("Free", &b2BlockAllocator::Free);
	blockAllocatorClass.function("Clear", &b2BlockAllocator::Clear);
	blockAllocatorClass.function("GetNumGiantAllocations", &b2BlockAllocator::GetNumGiantAllocations);
	b2mod.class_("BlockAllocator", blockAllocatorClass);

	
	v8pp::class_<b2Color> colorClass(isolate);
	colorClass.auto_wrap_objects();
	colorClass.ctor<float, float, float>();
	colorClass.function("Set", &b2Color::Set);
	colorClass.var("r", &b2Color::r);
	colorClass.var("g", &b2Color::g);
	colorClass.var("b", &b2Color::b);
	b2mod.class_("Color", colorClass);


	v8pp::class_<b2Draw> b2DrawClass(isolate);
	b2DrawClass.auto_wrap_objects();
	b2DrawClass.static_("shapeBit", 0x0001, true);
	b2DrawClass.static_("jointBit", 0x0002, true);
	b2DrawClass.static_("aabbBit", 0x0004, true);
	b2DrawClass.static_("pairBit", 0x0008, true);
	b2DrawClass.static_("centerOfMassBit", 0x0010, true);
	b2DrawClass.static_("particleBit", 0x0020, true);
	b2DrawClass.function("SetFlags", &b2Draw::SetFlags);
	b2DrawClass.function("GetFlags", &b2Draw::GetFlags);
	b2DrawClass.function("AppendFlags", &b2Draw::AppendFlags);
	b2DrawClass.function("ClearFlags", &b2Draw::ClearFlags);
	b2DrawClass.function("DrawPolygon", &b2Draw::DrawPolygon);
	b2DrawClass.function("DrawSolidPolygon", &b2Draw::DrawSolidPolygon);
	b2DrawClass.function("DrawCircle", &b2Draw::DrawCircle);
	b2DrawClass.function("DrawSolidCircle", &b2Draw::DrawSolidCircle);
	b2DrawClass.function("DrawParticles", &b2Draw::DrawParticles);
	b2DrawClass.function("DrawSegment", &b2Draw::DrawSegment);
	b2mod.class_("Draw",b2DrawClass);

	v8pp::class_<b2Stat> b2Stat_Class(isolate);
	b2Stat_Class.auto_wrap_objects();
	b2Stat_Class.ctor<>();
	b2Stat_Class.function("Record", &b2Stat::Record);
	b2Stat_Class.function("GetCount", &b2Stat::GetCount);
	b2Stat_Class.function("AppendFlags", &b2Stat::GetMean);
	b2Stat_Class.function("ClearFlags", &b2Stat::GetMin);
	b2Stat_Class.function("DrawPolygon", &b2Stat::GetMax);
	b2Stat_Class.function("DrawSolidPolygon", &b2Stat::Clear);
	b2mod.class_("Stat", b2Stat_Class);

	v8pp::class_<b2Timer> b2Timer_Class(isolate);
	b2Timer_Class.auto_wrap_objects();
	b2Timer_Class.ctor<>();
	b2Timer_Class.function("Reset", &b2Timer::Reset);
	b2Timer_Class.function("GetMilliseconds", &b2Timer::GetMilliseconds);
	b2mod.class_("Timer", b2Timer_Class);


	// b2Math.h
	b2mod.function("IsValid", b2IsValid);
	b2mod.function("b2InvSqrt", b2InvSqrt);


	v8pp::class_<b2Vec2> b2Vec2Class(isolate);
	b2Vec2Class.auto_wrap_objects();
	b2Vec2Class.ctor<float, float>([](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
		const auto isolate = args.GetIsolate();
		if (args.Length() == 0)
			return new b2Vec2();
		return new b2Vec2(v8pp::from_v8<float>(isolate, args[0]), v8pp::from_v8<float>(isolate, args[1]));
	});
	b2Vec2Class.function("SetZero", &b2Vec2::SetZero);
	b2Vec2Class.function("Set", &b2Vec2::Set);
	b2Vec2Class.function("Length", &b2Vec2::Length);
	b2Vec2Class.function("LengthSquared", &b2Vec2::LengthSquared);
	b2Vec2Class.function("Normalize", &b2Vec2::Normalize);
	b2Vec2Class.function("IsValid", &b2Vec2::IsValid);
	b2Vec2Class.function("Skew", &b2Vec2::Skew);
	b2Vec2Class.var("x", &b2Vec2::x);
	b2Vec2Class.var("y", &b2Vec2::y);
	b2Vec2Class.function("toJSON", [](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<b2Vec2>(isolate, args.This());
			return "[" + std::to_string(This.x) +", " + std::to_string(This.y) + "]";
	});
	b2mod.class_("Vec2", b2Vec2Class);

	v8pp::class_<b2Vec3> b2Vec3Class(isolate);
	b2Vec3Class.auto_wrap_objects();
	b2Vec3Class.ctor<float, float, float>([](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			if (args.Length() == 0)
				return new b2Vec3();
			return new b2Vec3(v8pp::from_v8<float>(isolate, args[0]),
				v8pp::from_v8<float>(isolate, args[1]),
				v8pp::from_v8<float>(isolate, args[2]));
		});
	b2Vec3Class.function("SetZero", &b2Vec3::SetZero);
	b2Vec3Class.function("Set", &b2Vec3::Set);
	b2Vec3Class.function("Length", &b2Vec3::Length);
	b2Vec3Class.function("Normalize", &b2Vec3::Normalize);
	b2Vec3Class.var("x", &b2Vec3::x);
	b2Vec3Class.var("y", &b2Vec3::y);
	b2Vec3Class.var("z", &b2Vec3::z);
	b2mod.class_("Vec3", b2Vec3Class);

	v8pp::class_<b2Vec4> b2Vec4Class(isolate);
	b2Vec4Class.ctor<float, float, float, float>([](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			if (args.Length() == 0)
				return new b2Vec4();
			return new b2Vec4(v8pp::from_v8<float>(isolate, args[0]),
				v8pp::from_v8<float>(isolate, args[1]),
				v8pp::from_v8<float>(isolate, args[2]),
				v8pp::from_v8<float>(isolate, args[3]));
		});
	b2Vec4Class.var("x", &b2Vec4::x);
	b2Vec4Class.var("y", &b2Vec4::y);
	b2Vec4Class.var("z", &b2Vec4::z);
	b2Vec4Class.var("w", &b2Vec4::w);
	b2mod.class_("Vec4", b2Vec4Class);

	v8pp::class_<b2Mat22> b2Mat22Class(isolate);
	b2Mat22Class.auto_wrap_objects();
	b2Mat22Class.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
		if (args.Length() == 0)
			return new b2Mat22();
		if (args.Length() == 2)
			return new b2Mat22( v8pp::from_v8<b2Vec2>(args.GetIsolate(), args[0]),
				v8pp::from_v8<b2Vec2>(args.GetIsolate(), args[1]));
		return new b2Mat22(v8pp::from_v8<float>(args.GetIsolate(), args[0]),
			v8pp::from_v8<float>(args.GetIsolate(), args[1]),
			v8pp::from_v8<float>(args.GetIsolate(), args[2]),
			v8pp::from_v8<float>(args.GetIsolate(), args[3]));
	});
	b2Mat22Class.function("Set", &b2Mat22::Set);
	b2Mat22Class.function("SetIdentity", &b2Mat22::SetIdentity);
	b2Mat22Class.function("SetZero", &b2Mat22::SetZero);
	b2Mat22Class.function("GetInverse", &b2Mat22::GetInverse);
	b2Mat22Class.function("Solve", &b2Mat22::Solve);
	b2Mat22Class.var("ex", &b2Mat22::ex);
	b2Mat22Class.var("ey", &b2Mat22::ey);
	b2mod.class_("Mat22", b2Mat22Class);

	v8pp::class_<b2Mat33> b2Mat33Class(isolate);
	b2Mat33Class.auto_wrap_objects();
	b2Mat33Class.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			if (args.Length() == 0)
				return new b2Mat33();
			if (args.Length() == 3)
				return new b2Mat33(v8pp::from_v8<b2Vec3>(args.GetIsolate(), args[0]),
					v8pp::from_v8<b2Vec3>(args.GetIsolate(), args[1]),
					v8pp::from_v8<b2Vec3>(args.GetIsolate(), args[2]));
		});
	b2Mat33Class.function("SetZero", &b2Mat33::SetZero);
	b2Mat33Class.function("Solve33", &b2Mat33::Solve33);
	b2Mat33Class.function("Solve22", &b2Mat33::Solve22);
	b2Mat33Class.function("GetInverse22", &b2Mat33::GetInverse22);
	b2Mat33Class.function("GetSymInverse33", &b2Mat33::GetSymInverse33);
	b2Mat33Class.var("ex", &b2Mat33::ex);
	b2Mat33Class.var("ey", &b2Mat33::ey);
	b2Mat33Class.var("ez", &b2Mat33::ez);
	b2mod.class_("Mat33", b2Mat33Class);

	v8pp::class_<b2Rot> b2RotClass(isolate);
	b2RotClass.auto_wrap_objects();
	b2RotClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
			if (args.Length() == 0)
				return new b2Rot();
			return new b2Rot(v8pp::from_v8<float>(args.GetIsolate(), args[0]));
	});
	b2RotClass.function("Set", &b2Rot::Set);
	b2RotClass.function("SetIdentity", &b2Rot::SetIdentity);
	b2RotClass.function("GetAngle", &b2Rot::GetAngle);
	b2RotClass.function("GetXAxis", &b2Rot::GetXAxis);
	b2RotClass.function("GetYAxis", &b2Rot::GetYAxis);
	b2RotClass.var("s", &b2Rot::s);
	b2RotClass.var("c", &b2Rot::c);
	b2mod.class_("Rot", b2RotClass);

	v8pp::class_<b2Transform> b2TransformClass(isolate);
	b2TransformClass.auto_wrap_objects();
	b2TransformClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			if (args.Length() == 0)
				return new b2Transform();
			return new b2Transform(v8pp::from_v8<b2Vec2>(args.GetIsolate(), args[0]),
				v8pp::from_v8<b2Rot>(args.GetIsolate(), args[1]));
		});
	b2TransformClass.function("SetIdentity", &b2Transform::SetIdentity);
	b2TransformClass.function("Set", &b2Transform::Set);
	b2TransformClass.var("p", &b2Transform::p);
	b2TransformClass.var("q", &b2Transform::q);
	b2mod.class_("Transform", b2TransformClass);

	v8pp::class_<b2Sweep> b2SweepClass(isolate);
	b2SweepClass.auto_wrap_objects();
	b2SweepClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
			return new b2Sweep();
	});
	b2SweepClass.function("GetTransform", &b2Sweep::GetTransform);
	b2SweepClass.function("Advance", &b2Sweep::Advance);
	b2SweepClass.function("Normalize", &b2Sweep::Normalize);
	b2SweepClass.var("localCenter", &b2Sweep::localCenter);
	b2SweepClass.var("c0", &b2Sweep::c0);
	b2SweepClass.var("c", &b2Sweep::c);
	b2SweepClass.var("a0", &b2Sweep::a0);
	b2SweepClass.var("a", &b2Sweep::a);
	b2SweepClass.var("alpha0", &b2Sweep::alpha0);
	b2mod.class_("Sweep", b2SweepClass);
	
	b2mod.function("Dot", [](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
			const auto isolate = args.GetIsolate();
			const auto ctx = isolate->GetCurrentContext();
			if (args[0]->InstanceOf(ctx, v8pp::class_<b2Vec2>::create_object(isolate)).ToChecked())
				return b2Dot(v8pp::from_v8<b2Vec2>(isolate, args[0]), v8pp::from_v8<b2Vec2>(isolate, args[1]));
			return b2Dot(v8pp::from_v8<b2Vec3>(isolate, args[0]), v8pp::from_v8<b2Vec3>(isolate, args[1]));
	});
	b2mod.function("Cross3", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
				return b2Cross(v8pp::from_v8<b2Vec3>(isolate, args[0]), v8pp::from_v8<b2Vec3>(isolate, args[1]));
		});
	b2mod.function("Cross2", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			return b2Cross(v8pp::from_v8<b2Vec2>(isolate, args[0]), v8pp::from_v8<b2Vec2>(isolate, args[1]));

		});
	b2mod.function("Cross", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto ctx = isolate->GetCurrentContext();
			if (args[0]->InstanceOf(ctx, v8pp::class_<b2Vec2>::create_object(isolate)).ToChecked())
			{
				return b2Cross(v8pp::from_v8<b2Vec2>(isolate, args[0]), v8pp::from_v8<float>(isolate, args[1]));
			}
			return b2Cross(v8pp::from_v8<float>(isolate, args[0]), v8pp::from_v8<b2Vec2>(isolate, args[1]));

		});

	b2mod.function("MulMat", static_cast<b2Mat22(*)(const b2Mat22&, const b2Mat22&)>(b2Mul));
	b2mod.function("MulRot", static_cast<b2Rot(*)(const b2Rot&, const b2Rot&)>(b2Mul));
	b2mod.function("MulTransform", static_cast<b2Transform(*)(const b2Transform&, const b2Transform&)>(b2Mul));
	b2mod.function("MulMatVec", static_cast<b2Vec2(*)(const b2Mat22& A, const b2Vec2& v)>(b2Mul));
	b2mod.function("MulRotVec", static_cast<b2Vec2(*)(const b2Rot&, const b2Vec2&)>(b2Mul));
	b2mod.function("MulTransformVec", static_cast<b2Vec2(*)(const b2Transform&, const b2Vec2&)>(b2Mul));

	b2mod.function("MulTMat", static_cast<b2Mat22(*)(const b2Mat22&, const b2Mat22&)>(b2MulT));
	b2mod.function("MulTRot", static_cast<b2Rot(*)(const b2Rot&, const b2Rot&)>(b2MulT));
	b2mod.function("MulTTransform", static_cast<b2Transform(*)(const b2Transform&, const b2Transform&)>(b2MulT));
	b2mod.function("MulTMatVec", static_cast<b2Vec2(*)(const b2Mat22& A, const b2Vec2& v)>(b2MulT));
	b2mod.function("MulTRotVec", static_cast<b2Vec2(*)(const b2Rot&, const b2Vec2&)>(b2MulT));
	b2mod.function("MulTTransformVec", static_cast<b2Vec2(*)(const b2Transform&, const b2Vec2&)>(b2MulT));

	b2mod.function("Distance", static_cast<float(*)(const b2Vec2&, const b2Vec2&)>(b2Distance));
	b2mod.function("DistanceSquared", b2DistanceSquared);

	// ... help meh!
	// TODO: some missing funs

	b2mod.function("NextPowerOfTwo", b2NextPowerOfTwo);
	b2mod.function("IsPowerOfTwo", b2IsPowerOfTwo);

	// b2Collission.h

	v8pp::class_<b2ContactFeature> b2ContactFeatureClass(isolate);
	b2ContactFeatureClass.auto_wrap_objects();
	b2ContactFeatureClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
			return new b2ContactFeature();
	});
	b2ContactFeatureClass.static_("vertex", 0, true);
	b2ContactFeatureClass.static_("face", 1, true);
	b2ContactFeatureClass.var("indexA", &b2ContactFeature::indexA);
	b2ContactFeatureClass.var("indexB", &b2ContactFeature::indexB);
	b2ContactFeatureClass.var("typeA", &b2ContactFeature::typeA);
	b2ContactFeatureClass.var("typeB", &b2ContactFeature::typeB);
	b2mod.class_("ContactFeature", b2ContactFeatureClass);

	//v8pp::class_<b2ContactID> b2ContactIDClass(isolate);
	//b2ContactIDClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
	//	{
	//		return new b2ContactID();
	//	});
	//b2ContactIDClass.var("cf", &b2ContactID::cf);
	//b2ContactIDClass.var("key", &b2ContactID::key);
	//b2mod.class_("ContactID", b2ContactIDClass);

	
	v8pp::class_<b2ManifoldPoint> b2ManifoldPointClass(isolate);
	b2ManifoldPointClass.auto_wrap_objects();
	b2ManifoldPointClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			return new b2ManifoldPoint();
		});
	b2ManifoldPointClass.var("localPoint", &b2ManifoldPoint::localPoint);
	b2ManifoldPointClass.var("normalImpulse", &b2ManifoldPoint::normalImpulse);
	b2ManifoldPointClass.var("tangentImpulse", &b2ManifoldPoint::tangentImpulse);
	//b2ManifoldPointClass.var("id", &b2ManifoldPoint::id);
	b2mod.class_("ManifoldPoint", b2ManifoldPointClass);

	v8pp::class_<b2Manifold> b2ManifoldClass(isolate);
	b2ManifoldClass.auto_wrap_objects();
	b2ManifoldClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			return new b2Manifold();
		});
	b2ManifoldClass.static_("circles", b2Manifold::Type::e_circles, true);
	b2ManifoldClass.static_("faceA", b2Manifold::Type::e_faceA, true);
	b2ManifoldClass.static_("faceB", b2Manifold::Type::e_faceB, true);
	b2ManifoldClass.property("points", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = args.This();
			const auto nThis = v8pp::from_v8<b2Manifold>(isolate, This);
			std::vector<b2ManifoldPoint> res;
			res.reserve(b2_maxManifoldPoints);
			for (auto i = 0; i < b2_maxManifoldPoints; i++)
				res.push_back(nThis.points[i]);
			return res;
		},
		[](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = args.This();
			auto nThis = v8pp::from_v8<b2Manifold>(isolate, This);
			const auto vec = v8pp::from_v8<std::vector<b2ManifoldPoint>>(isolate, args[0]);
			for (auto i = 0; i < b2_maxManifoldPoints; i++)
				nThis.points[i] = vec[i];
		});
	b2ManifoldClass.var("localNormal", &b2Manifold::localNormal);
	b2ManifoldClass.var("localPoint", &b2Manifold::localPoint);
	b2ManifoldClass.var("type", &b2Manifold::type);
	b2ManifoldClass.var("pointCount", &b2Manifold::pointCount);
	b2mod.class_("Manifold", b2ManifoldClass);

	v8pp::class_<b2WorldManifold> b2WorldManifoldClass(isolate);
	b2WorldManifoldClass.auto_wrap_objects();
	b2WorldManifoldClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
			return new b2WorldManifold();
	});
	b2WorldManifoldClass.function("Initialize", &b2WorldManifold::Initialize);
	b2WorldManifoldClass.var("normal", &b2WorldManifold::normal);
	b2ManifoldClass.property("points", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = args.This();
			const auto nThis = v8pp::from_v8<b2WorldManifold>(isolate, This);
			std::vector<b2Vec2> res;
			res.reserve(b2_maxManifoldPoints);
			for (auto i = 0; i < b2_maxManifoldPoints; i++)
				res.push_back(nThis.points[i]);
			return res;
		},
		[](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = args.This();
			auto nThis = v8pp::from_v8<b2WorldManifold>(isolate, This);
			const auto vec = v8pp::from_v8<std::vector<b2Vec2>>(isolate, args[0]);
			for (auto i = 0; i < b2_maxManifoldPoints; i++)
				nThis.points[i] = vec[i];
		});
	b2ManifoldClass.property("separations", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = args.This();
			const auto nThis = v8pp::from_v8<b2WorldManifold>(isolate, This);
			std::vector<float32> res;
			res.reserve(b2_maxManifoldPoints);
			for (auto i = 0; i < b2_maxManifoldPoints; i++)
				res.push_back(nThis.separations[i]);
			return res;
		},
		[](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = args.This();
			auto nThis = v8pp::from_v8<b2WorldManifold>(isolate, This);
			const auto vec = v8pp::from_v8<std::vector<float32>>(isolate, args[0]);
			for (auto i = 0; i < b2_maxManifoldPoints; i++)
				nThis.separations[i] = vec[i];
		});
	b2mod.class_("WorldManifold", b2WorldManifoldClass);

	v8pp::module b2PointStateMod(isolate);
	b2PointStateMod.const_("nullState", b2PointState::b2_nullState);
	b2PointStateMod.const_("addState", b2PointState::b2_addState);
	b2PointStateMod.const_("persistState", b2PointState::b2_persistState);
	b2PointStateMod.const_("removeState", b2PointState::b2_removeState);
	b2mod.submodule("PointState", b2PointStateMod);

	b2mod.function("GetPointStates", [](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
		const auto isolate = args.GetIsolate();
		auto state1 = v8pp::from_v8<std::vector<b2PointState>>(isolate, args[0]);
		auto state2 = v8pp::from_v8<std::vector<b2PointState>>(isolate, args[1]);
		const auto manifold1 = v8pp::from_v8<b2Manifold>(isolate, args[2]);
		const auto manifold2 = v8pp::from_v8<b2Manifold>(isolate, args[3]);
		b2GetPointStates(state1.data(), state2.data(), &manifold1, &manifold2);
		args[0] = v8pp::to_v8(isolate, state1);
		args[1] = v8pp::to_v8(isolate, state2);
	});


	v8pp::class_<b2ClipVertex> b2ClipVertexClass(isolate);
	b2ClipVertexClass.auto_wrap_objects();
	b2ClipVertexClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
			return new b2ClipVertex();
	});
	b2ClipVertexClass.var("v", &b2ClipVertex::v);
	//b2ClipVertexClass.var("id", &b2ClipVertex::id);
	b2mod.class_("ClipVertex", b2ClipVertexClass);


	v8pp::class_<b2RayCastInput> b2RayCastInputClass(isolate);
	b2RayCastInputClass.auto_wrap_objects();
	b2RayCastInputClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			return new b2RayCastInput();
		});
	b2RayCastInputClass.var("p1", &b2RayCastInput::p1);
	b2RayCastInputClass.var("p2", &b2RayCastInput::p2);
	b2RayCastInputClass.var("maxFraction", &b2RayCastInput::maxFraction);
	b2mod.class_("RayCastInput", b2RayCastInputClass);


	v8pp::class_<b2RayCastOutput> b2RayCastOutputClass(isolate);
	b2RayCastInputClass.auto_wrap_objects();
	b2RayCastOutputClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			return new b2RayCastOutput();
		});
	b2RayCastOutputClass.var("normal", &b2RayCastOutput::normal);
	b2RayCastOutputClass.var("fraction", &b2RayCastOutput::fraction);
	b2mod.class_("RayCastOutput", b2RayCastOutputClass);

	v8pp::class_<b2AABB> b2AABBClass(isolate);
	b2AABBClass.auto_wrap_objects();
	b2AABBClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			return new b2AABB();
		});
	b2AABBClass.function("IsValid", &b2AABB::IsValid);
	b2AABBClass.function("GetCenter", &b2AABB::GetCenter);
	b2AABBClass.function("GetExtents", &b2AABB::GetExtents);
	b2AABBClass.function("GetPerimeter", &b2AABB::GetPerimeter);
	b2AABBClass.function("Combine", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = args.This();
			auto nThis = v8pp::from_v8<b2AABB>(isolate, This);
			if (args.Length() == 1)
				return nThis.Combine(v8pp::from_v8<b2AABB>(isolate, args[0]));
			return nThis.Combine(v8pp::from_v8<b2AABB>(isolate, args[0]), v8pp::from_v8<b2AABB>(isolate, args[1]));
		});
	b2AABBClass.function("Contains", &b2AABB::Contains);
	b2AABBClass.function("RayCast", &b2AABB::RayCast);
	b2AABBClass.var("lowerBound", &b2AABB::lowerBound);
	b2AABBClass.var("upperBound", &b2AABB::upperBound);
	b2mod.class_("AABB", b2AABBClass);

	b2mod.function("CollideCircles", b2CollideCircles);
	b2mod.function("CollidePolygonAndCircle", b2CollidePolygonAndCircle);
	b2mod.function("CollidePolygons", b2CollidePolygons);
	b2mod.function("CollideEdgeAndCircle", b2CollideEdgeAndCircle);
	b2mod.function("CollideEdgeAndPolygon", b2CollideEdgeAndPolygon);
	b2mod.function("ClipSegmentToLine", b2ClipSegmentToLine);
	b2mod.function("TestOverlap", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			if (args.Length() == 2)
				return b2TestOverlap(v8pp::from_v8<b2AABB>(isolate, args[0]), v8pp::from_v8<b2AABB>(isolate, args[1]));
			return b2TestOverlap(v8pp::from_v8<const b2Shape*>(isolate, args[0]), 
				v8pp::from_v8<int>(isolate, args[1]),
				v8pp::from_v8<const b2Shape*>(isolate, args[2]),
				v8pp::from_v8<int>(isolate, args[3]),
				v8pp::from_v8<b2Transform>(isolate, args[4]),
				v8pp::from_v8<b2Transform>(isolate, args[5]));
		
		});

	// b2Shape.h
	
	v8pp::class_<b2MassData> b2MassDataClass(isolate);
	b2ManifoldClass.auto_wrap_objects();
	b2MassDataClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
			return new b2MassData();
	});
	b2MassDataClass.var("mass", &b2MassData::mass);
	b2MassDataClass.var("center", &b2MassData::center);
	b2MassDataClass.var("I", &b2MassData::I);
	b2mod.class_("MassData", b2MassDataClass);

	
	v8pp::class_<b2Shape> b2ShapeClass(isolate);
	b2ShapeClass.auto_wrap_objects();
	b2ShapeClass.function("Clone", &b2Shape::Clone);
	b2ShapeClass.function("GetType", &b2Shape::GetType);
	b2ShapeClass.function("GetChildCount", &b2Shape::GetChildCount);
	b2ShapeClass.static_("circle", 0, true);
	b2ShapeClass.static_("edge", 1, true);
	b2ShapeClass.static_("polygon", 2, true);
	b2ShapeClass.static_("chain", 3, true);
	b2ShapeClass.static_("typeCount", 4, true);
	b2ShapeClass.function("TestPoint", &b2Shape::TestPoint);
	b2ShapeClass.function("ComputeDistance", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<b2Shape*>(isolate, args.This());
			auto xf = v8pp::from_v8<b2Transform>(isolate, args[0]);
			auto p = v8pp::from_v8<b2Vec2>(isolate, args[1]);
			auto distance = v8pp::from_v8<float32>(isolate, args[2]);
			auto normal = v8pp::from_v8<b2Vec2>(isolate, args[3]);
			This->ComputeDistance(
				xf,
				p,
				&distance,
				&normal,
				v8pp::from_v8<int32>(isolate, args[4])
			);
			args[0] = v8pp::to_v8(isolate, xf);
			args[1] = v8pp::to_v8(isolate, p);
			args[2] = v8pp::to_v8(isolate, distance);
			args[3] = v8pp::to_v8(isolate, normal);
		});
	b2ShapeClass.function("RayCast", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<b2Shape*>(isolate, args.This());
			auto output = v8pp::from_v8<b2RayCastOutput>(isolate, args[0]);
			const auto input = v8pp::from_v8<b2RayCastInput>(isolate, args[1]);
			const auto transform = v8pp::from_v8<b2Transform>(isolate, args[2]);
			This->RayCast(
				&output,
				input,
				transform,
				v8pp::from_v8<int32>(isolate, args[3])
			);
			args[0] = v8pp::to_v8(isolate, output);
			args[1] = v8pp::to_v8(isolate, input);
			args[2] = v8pp::to_v8(isolate, transform);
		});
	b2ShapeClass.function("ComputeAABB", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<b2Shape*>(isolate, args.This());
			auto aabb = v8pp::from_v8<b2AABB>(isolate, args[0]);
			const auto transform = v8pp::from_v8<b2Transform>(isolate, args[1]);
			This->ComputeAABB(
				&aabb,
				transform,
				v8pp::from_v8<int32>(isolate, args[2])
			);
			args[0] = v8pp::to_v8(isolate, aabb);
			args[1] = v8pp::to_v8(isolate, transform);
		});
	b2ShapeClass.function("ComputeMass", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<b2Shape*>(isolate, args.This());
			auto massData = v8pp::from_v8<b2MassData>(isolate, args[0]);
			This->ComputeMass(
				&massData,
				v8pp::from_v8<float32>(isolate, args[1])
			);
			args[0] = v8pp::to_v8(isolate, massData);
		});
	b2ShapeClass.var("m_type", &b2Shape::m_type);
	b2ShapeClass.var("m_radius", &b2Shape::m_radius);
	b2mod.class_("Shape", b2ShapeClass);

	v8pp::class_<b2CircleShape> b2CircleShapeClass(isolate);
	b2CircleShapeClass.auto_wrap_objects();
	b2CircleShapeClass.inherit<b2Shape>();
	b2CircleShapeClass.ctor<>();
	b2CircleShapeClass.function("Clone", &b2CircleShape::Clone);
	b2CircleShapeClass.function("GetChildCount", &b2CircleShape::GetChildCount);
	b2CircleShapeClass.function("TestPoint", &b2CircleShape::TestPoint);
	b2CircleShapeClass.function("GetSupport", &b2CircleShape::GetSupport);
	b2CircleShapeClass.function("GetSupportVertex", &b2CircleShape::GetSupportVertex);
	b2CircleShapeClass.function("GetVertexCount", &b2CircleShape::GetVertexCount);
	b2CircleShapeClass.function("GetVertex", &b2CircleShape::GetVertex);
	b2CircleShapeClass.var("m_p", &b2CircleShape::m_p);
	b2mod.class_("CircleShape", b2CircleShapeClass);

	v8pp::class_<b2EdgeShape> b2EdgeShapeClass(isolate);
	b2EdgeShapeClass.auto_wrap_objects();
	b2EdgeShapeClass.inherit<b2Shape>();
	b2EdgeShapeClass.ctor<>();
	b2EdgeShapeClass.var("m_vertex1", &b2EdgeShape::m_vertex1);
	b2EdgeShapeClass.var("m_vertex2", &b2EdgeShape::m_vertex2);
	b2EdgeShapeClass.var("m_vertex0", &b2EdgeShape::m_vertex0);
	b2EdgeShapeClass.var("m_vertex3", &b2EdgeShape::m_vertex3);
	b2EdgeShapeClass.var("m_hasVertex0", &b2EdgeShape::m_hasVertex0);
	b2EdgeShapeClass.var("m_hasVertex3", &b2EdgeShape::m_hasVertex3);
	b2mod.class_("EdgeShape", b2EdgeShapeClass);

	v8pp::class_<b2ChainShape> b2ChainShapeClass(isolate);
	b2ChainShapeClass.auto_wrap_objects();
	b2ChainShapeClass.inherit<b2Shape>();
	b2ChainShapeClass.ctor<>();
	b2ChainShapeClass.function("CreateLoop", &b2ChainShape::CreateLoop);
	b2ChainShapeClass.function("CreateChain", &b2ChainShape::CreateChain);
	b2ChainShapeClass.function("SetPrevVertex", &b2ChainShape::SetPrevVertex);
	b2ChainShapeClass.function("SetNextVertex", &b2ChainShape::SetNextVertex);
	b2ChainShapeClass.function("GetChildEdge", &b2ChainShape::GetChildEdge);
	b2ChainShapeClass.property("m_vertices", [](v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
		{
			const auto isolate = info.GetIsolate();
			const auto This = info.This();
			const auto nThis = v8pp::from_v8<b2ChainShape*>(isolate, This);
			std::vector<b2Vec2> res;
			for (auto i = 0; i < nThis->m_count; i++)
				res.push_back(nThis->m_vertices[i]);
			return res;
		},
		[](v8::Local<v8::String> property, v8::Local <v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
		{
			const auto isolate = info.GetIsolate();
			const auto This = info.This();
			auto nThis = v8pp::from_v8<b2ChainShape*>(isolate, This);
			const auto vec = v8pp::from_v8<std::vector<b2Vec2>>(isolate, value);
			for (auto i = 0; i < nThis->m_count; i++)
				nThis->m_vertices[i] = vec[i];
		});
	b2ChainShapeClass.var("m_count", &b2ChainShape::m_count);
	b2ChainShapeClass.var("m_prevVertex", &b2ChainShape::m_prevVertex);
	b2ChainShapeClass.var("m_nextVertex", &b2ChainShape::m_nextVertex);
	b2ChainShapeClass.var("m_hasPrevVertex", &b2ChainShape::m_hasPrevVertex);
	b2ChainShapeClass.var("m_hasNextVertex", &b2ChainShape::m_hasNextVertex);
	b2mod.class_("ChainShape", b2ChainShapeClass);

	v8pp::class_<b2PolygonShape> b2PolygonShapeClass(isolate);
	b2PolygonShapeClass.auto_wrap_objects();
	b2PolygonShapeClass.inherit<b2Shape>();
	b2PolygonShapeClass.ctor<>();
	b2PolygonShapeClass.function("Set", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
				const auto isolate = args.GetIsolate();
				auto This = v8pp::from_v8<b2PolygonShape*>(isolate, args.This());
				v8::Local<v8::Array> points = args[0].As<v8::Array>();
				std::vector<b2Vec2> nPoints;
				nPoints.reserve(points->Length());
				for (auto i = 0; i < points->Length(); i++)
					nPoints.push_back(
						v8pp::from_v8<b2Vec2>(isolate, points->Get(isolate->GetCurrentContext(), i)
							.ToLocalChecked()));
				This->Set(nPoints.data(), v8pp::from_v8<int32>(isolate, args[1]));
		});
	b2PolygonShapeClass.function("SetAsBox", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			auto This = v8pp::from_v8<b2PolygonShape*>(isolate, args.This());
			if (args.Length() == 2)
				return This->SetAsBox(v8pp::from_v8<float32>(isolate, args[0]), v8pp::from_v8<float32>(isolate, args[1]));
			return This->SetAsBox(
				v8pp::from_v8<float32>(isolate, args[0]),
				v8pp::from_v8<float32>(isolate, args[1]),
				v8pp::from_v8<b2Vec2>(isolate, args[2]),
				v8pp::from_v8<float32>(isolate, args[3]));
		});
	b2PolygonShapeClass.function("GetVertexCount", &b2PolygonShape::GetVertexCount);
	b2PolygonShapeClass.function("GetVertex", &b2PolygonShape::GetVertex);
	b2PolygonShapeClass.function("Validate", &b2PolygonShape::Validate);
	b2PolygonShapeClass.var("m_centroid", &b2PolygonShape::m_centroid);
	b2PolygonShapeClass.property("m_vertices", [](v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
		{
			const auto isolate = info.GetIsolate();
			const auto This = info.This();
			const auto nThis = v8pp::from_v8<b2PolygonShape*>(isolate, This);
			std::vector<b2Vec2> res;
			for (auto i = 0; i < nThis->m_count; i++)
				res.push_back(nThis->m_vertices[i]);
			return res;
		},
		[](v8::Local<v8::String> property, v8::Local <v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
		{
			const auto isolate = info.GetIsolate();
			const auto This = info.This();
			auto nThis = v8pp::from_v8<b2PolygonShape*>(isolate, This);
			const auto vec = v8pp::from_v8<std::vector<b2Vec2>>(isolate, value);
			for (auto i = 0; i < nThis->m_count; i++)
				nThis->m_vertices[i] = vec[i];
		});
	b2PolygonShapeClass.property("m_normals", [](v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
		{
			const auto isolate = info.GetIsolate();
			const auto This = info.This();
			const auto nThis = v8pp::from_v8<b2PolygonShape*>(isolate, This);
			std::vector<b2Vec2> res;
			for (auto i = 0; i < nThis->m_count; i++)
				res.push_back(nThis->m_normals[i]);
			return res;
		},
		[](v8::Local<v8::String> property, v8::Local <v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
		{
			const auto isolate = info.GetIsolate();
			const auto This = info.This();
			auto nThis = v8pp::from_v8<b2PolygonShape*>(isolate, This);
			const auto vec = v8pp::from_v8<std::vector<b2Vec2>>(isolate, value);
			for (auto i = 0; i < nThis->m_count; i++)
				nThis->m_normals[i] = vec[i];
		});
	b2PolygonShapeClass.var("m_count", &b2PolygonShape::m_count);
	b2mod.class_("PolygonShape", b2PolygonShapeClass);

	//#include <Box2D/Collision/b2BroadPhase.h>
	//#include <Box2D/Collision/b2Distance.h>
	//#include <Box2D/Collision/b2DynamicTree.h>
	//#include <Box2D/Collision/b2TimeOfImpact.h>
	
	// b2body.h
	
	v8pp::module b2BodyTypeMod(isolate);
	b2BodyTypeMod.const_("staticBody", b2BodyType::b2_staticBody);
	b2BodyTypeMod.const_("kinematicBody", b2BodyType::b2_kinematicBody);
	b2BodyTypeMod.const_("dynamicBody", b2BodyType::b2_dynamicBody);
	b2mod.submodule("BodyType", b2BodyTypeMod);

	v8pp::class_<b2BodyDef> b2BodyDefClass(isolate);
	b2BodyDefClass.auto_wrap_objects();
	b2BodyDefClass.ctor<>();
	b2BodyDefClass.var("type", &b2BodyDef::type);
	//b2BodyDefClass.var("position", &b2BodyDef::position);
	b2BodyDefClass.property("position",[](v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
			b2BodyDef* This = v8pp::from_v8<b2BodyDef*>(info.GetIsolate(), info.This());
			b2Vec2* vecRef = &(This->position);
			info.GetReturnValue().Set(v8pp::to_v8(info.GetIsolate(), vecRef));
		}, [](v8::Local<v8::String> property, v8::Local <v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8pp::from_v8<b2BodyDef*>(info.GetIsolate(), info.This())->position = v8pp::from_v8<b2Vec2>(info.GetIsolate(), value);
	});
	b2BodyDefClass.var("angle", &b2BodyDef::angle);
	b2BodyDefClass.var("linearVelocity", &b2BodyDef::linearVelocity);
	b2BodyDefClass.var("angularVelocity", &b2BodyDef::angularVelocity);
	b2BodyDefClass.var("linearDamping", &b2BodyDef::linearDamping);
	b2BodyDefClass.var("angularDamping", &b2BodyDef::angularDamping);
	b2BodyDefClass.var("allowSleep", &b2BodyDef::allowSleep);
	b2BodyDefClass.var("awake", &b2BodyDef::awake);
	b2BodyDefClass.var("fixedRotation", &b2BodyDef::fixedRotation);
	b2BodyDefClass.var("bullet", &b2BodyDef::bullet);
	b2BodyDefClass.var("active", &b2BodyDef::active);
	// TODO: cast void Ptr:  b2BodyDefClass.var("userData", &b2BodyDef::userData);
	b2BodyDefClass.var("gravityScale", &b2BodyDef::gravityScale);
	b2mod.class_("BodyDef", b2BodyDefClass);

	v8pp::class_<b2Body> b2BodyClass(isolate, [](v8::Isolate* isolate, b2Body* const& obj)
		{
			// will be destroyed by world
		});
	b2BodyClass.auto_wrap_objects();
	b2BodyClass.function("CreateFixture", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			auto This = v8pp::from_v8<b2Body*>(isolate, args.This());
			auto fixtureDef = v8pp::from_v8<b2FixtureDef*>(isolate, args[0]);
			if (args.Length() == 1)
				return This->CreateFixture(fixtureDef);
			return This->CreateFixture(v8pp::from_v8<b2Shape*>(isolate, args[0]), v8pp::from_v8<float>(isolate, args[1]));
		});
	b2BodyClass.function("DestroyFixture", &b2Body::DestroyFixture);
	b2BodyClass.function("SetTransform", &b2Body::SetTransform);
	b2BodyClass.function("GetTransform", &b2Body::GetTransform);
	b2BodyClass.function("GetPosition", &b2Body::GetPosition);
	b2BodyClass.function("GetAngle", &b2Body::GetAngle);
	b2BodyClass.function("GetWorldCenter", &b2Body::GetWorldCenter);
	b2BodyClass.function("GetLocalCenter", &b2Body::GetLocalCenter);
	b2BodyClass.function("SetLinearVelocity", &b2Body::SetLinearVelocity);
	b2BodyClass.function("GetLinearVelocity", &b2Body::GetLinearVelocity);
	b2BodyClass.function("SetAngularVelocity", &b2Body::SetAngularVelocity);
	b2BodyClass.function("GetAngularVelocity", &b2Body::GetAngularVelocity);
	b2BodyClass.function("ApplyForce", &b2Body::ApplyForce);
	b2BodyClass.function("ApplyForceToCenter", &b2Body::ApplyForceToCenter);
	b2BodyClass.function("ApplyTorque", &b2Body::ApplyTorque);
	b2BodyClass.function("ApplyLinearImpulse", &b2Body::ApplyLinearImpulse);
	b2BodyClass.function("ApplyAngularImpulse", &b2Body::ApplyAngularImpulse);
	b2BodyClass.function("GetMass", &b2Body::GetMass);
	b2BodyClass.function("GetInertia", &b2Body::GetInertia);
	b2BodyClass.function("GetMassData", &b2Body::GetMassData);
	b2BodyClass.function("SetMassData", &b2Body::SetMassData);
	b2BodyClass.function("ResetMassData", &b2Body::ResetMassData);
	b2BodyClass.function("GetWorldPoint", &b2Body::GetWorldPoint);
	b2BodyClass.function("GetWorldVector", &b2Body::GetWorldVector);
	b2BodyClass.function("GetLocalPoint", &b2Body::GetLocalPoint);
	b2BodyClass.function("GetLocalVector", &b2Body::GetLocalVector);
	b2BodyClass.function("GetLinearVelocityFromWorldPoint", &b2Body::GetLinearVelocityFromWorldPoint);
	b2BodyClass.function("GetLinearVelocityFromLocalPoint", &b2Body::GetLinearVelocityFromLocalPoint);
	b2BodyClass.function("GetLinearDamping", &b2Body::GetLinearDamping);
	b2BodyClass.function("SetLinearDamping", &b2Body::SetLinearDamping);
	b2BodyClass.function("GetAngularDamping", &b2Body::GetAngularDamping);
	b2BodyClass.function("SetAngularDamping", &b2Body::SetAngularDamping);
	b2BodyClass.function("GetGravityScale", &b2Body::GetGravityScale);
	b2BodyClass.function("SetGravityScale", &b2Body::SetGravityScale);
	b2BodyClass.function("SetType", &b2Body::SetType);
	b2BodyClass.function("GetType", &b2Body::GetType);
	b2BodyClass.function("SetBullet", &b2Body::SetBullet);
	b2BodyClass.function("IsBullet", &b2Body::IsBullet);
	b2BodyClass.function("SetSleepingAllowed", &b2Body::SetSleepingAllowed);
	b2BodyClass.function("IsSleepingAllowed", &b2Body::IsSleepingAllowed);
	b2BodyClass.function("SetAwake", &b2Body::SetAwake);
	b2BodyClass.function("IsAwake", &b2Body::IsAwake);
	b2BodyClass.function("SetActive", &b2Body::SetActive);
	b2BodyClass.function("IsActive", &b2Body::IsActive);
	b2BodyClass.function("SetFixedRotation", &b2Body::SetFixedRotation);
	b2BodyClass.function("IsFixedRotation", &b2Body::IsFixedRotation);
	// TODO: b2BodyClass.function("GetFixtureList", &b2Body::GetFixtureList);
	// TODO: b2BodyClass.function("GetJointList", &b2Body::GetJointList);
	// TODO: b2BodyClass.function("GetContactList", &b2Body::GetContactList);
	b2BodyClass.function("GetNext",[](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<b2Body*>(isolate, args.This());
			return This->GetNext();
		});
	// TODO: change if using something else as user data
	b2BodyClass.function("GetUserData", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<b2Body*>(isolate, args.This());
			return static_cast<fse::FSEObject*>(This->GetUserData());
		});
	// TODO: change if using something else as user data
	b2BodyClass.function("SetUserData", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<b2Body*>(isolate, args.This());
			const auto obj = v8pp::from_v8<fse::FSEObject*>(isolate, args[0]);
			This->SetUserData(obj);
		});
	b2BodyClass.function("GetWorld", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<b2Body*>(isolate, args.This());
			return This->GetWorld();
		});
	b2BodyClass.function("Dump", &b2Body::Dump);
	b2mod.class_("Body", b2BodyClass);

	v8pp::class_<b2Filter> b2FilterClass(isolate);
	b2FilterClass.auto_wrap_objects();
	b2FilterClass.ctor<>();
	b2FilterClass.var("categoryBits", &b2Filter::categoryBits);
	b2FilterClass.var("maskBits", &b2Filter::maskBits);
	b2FilterClass.var("groupIndex", &b2Filter::groupIndex);
	b2mod.class_("Filter", b2FilterClass);

	v8pp::class_<b2FixtureDef> b2FixtureDefClass(isolate);
	b2FixtureDefClass.auto_wrap_objects();
	b2FixtureDefClass.ctor<>();
	b2FixtureDefClass.var("shape", &b2FixtureDef::shape);
	//b2FixtureDefClass.property("shape", [](v8::FunctionCallbackInfo<v8::Value> const& args)
	//	{
	//		const auto isolate = args.GetIsolate();
	//		const auto This = v8pp::from_v8<b2FixtureDef>(isolate, args.This());
	//		return This.shape;
	//	}, [](v8::FunctionCallbackInfo<v8::Value> const& args)
	//	{
	//		const auto isolate = args.GetIsolate();
	//		auto This = v8pp::from_v8<b2FixtureDef>(isolate, args.This());
	//		const auto shape = v8pp::from_v8<b2Shape*>(isolate, args[0]);
	//		This.shape = shape;
	//	});
	// TODO: cast voidPtr b2FixtureDefClass.var("userData", &b2FixtureDef::userData); 
	b2FixtureDefClass.var("friction", &b2FixtureDef::friction);
	b2FixtureDefClass.var("restitution", &b2FixtureDef::restitution);
	b2FixtureDefClass.var("density", &b2FixtureDef::density);
	b2FixtureDefClass.var("isSensor", &b2FixtureDef::isSensor);
	b2FixtureDefClass.var("filter", &b2FixtureDef::filter);
	b2mod.class_("FixtureDef", b2FixtureDefClass);

	v8pp::class_<b2FixtureProxy> b2FixtureProxyClass(isolate);
	b2FixtureProxyClass.auto_wrap_objects();
	b2FixtureProxyClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
			return new b2FixtureProxy();
	});
	b2FixtureProxyClass.var("aabb", &b2FixtureProxy::aabb);
	b2FixtureProxyClass.var("fixture", &b2FixtureProxy::fixture);
	b2FixtureProxyClass.var("childIndex", &b2FixtureProxy::childIndex);
	b2FixtureProxyClass.var("proxyId", &b2FixtureProxy::proxyId);
	b2mod.class_("FixtureProxy", b2FixtureProxyClass);

	v8pp::class_<b2Fixture> b2FixtureClass(isolate, [](v8::Isolate* isolate, b2Fixture* fixture)
	{
			// will be destroyed by body
	});
	b2FixtureClass.auto_wrap_objects();
	b2FixtureClass.function("GetType", &b2Fixture::GetType);
	b2FixtureClass.function("GetShape", static_cast<b2Shape*(b2Fixture::*)()>(&b2Fixture::GetShape));
	b2FixtureClass.function("SetSensor", &b2Fixture::SetSensor);
	b2FixtureClass.function("IsSensor", &b2Fixture::IsSensor);
	b2FixtureClass.function("SetFilterData", &b2Fixture::SetFilterData);
	b2FixtureClass.function("GetFilterData", &b2Fixture::GetFilterData);
	b2FixtureClass.function("Refilter", &b2Fixture::Refilter);
	b2FixtureClass.function("GetBody", static_cast<b2Body* (b2Fixture::*)()>(&b2Fixture::GetBody));
	b2FixtureClass.function("GetNext", static_cast<b2Fixture* (b2Fixture::*)()>(&b2Fixture::GetNext));
	// TODO: cast void ptr b2FixtureClass.function("GetUserData", &b2Fixture::GetUserData);
	// TODO: cast void ptr b2FixtureClass.function("SetUserData", &b2Fixture::SetUserData);
	b2FixtureClass.function("TestPoint", &b2Fixture::TestPoint);
	b2FixtureClass.function("ComputeDistance", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<b2Fixture>(isolate, args.This());
			auto distance = v8pp::from_v8<float>(isolate, args[1]);
			auto normal = v8pp::from_v8<b2Vec2>(isolate, args[2]);
			This.ComputeDistance(v8pp::from_v8<b2Vec2>(isolate, args[0]),
				&distance, &normal, v8pp::from_v8<int32>(isolate, args[3]));
			args[1] = v8pp::to_v8(isolate, distance);
			args[2] = v8pp::to_v8(isolate, normal);
		});
	b2FixtureClass.function("RayCast", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<b2Fixture>(isolate, args.This());
			auto output = v8pp::from_v8<b2RayCastOutput*>(isolate, args[0]);
			This.RayCast(output, v8pp::from_v8<b2RayCastInput>(isolate, args[1]), v8pp::from_v8<int32>(isolate, args[2]));
			args[0] = v8pp::to_v8(isolate, output);
		});
	b2FixtureClass.function("GetMassData", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<b2Fixture>(isolate, args.This());
			auto massData = v8pp::from_v8<b2MassData*>(isolate, args[0]);
			This.GetMassData(massData);
			args[0] = v8pp::to_v8(isolate, massData);
		});
	b2FixtureClass.function("SetDensity", &b2Fixture::SetDensity);
	b2FixtureClass.function("GetDensity", &b2Fixture::GetDensity);
	b2FixtureClass.function("GetFriction", &b2Fixture::GetFriction);
	b2FixtureClass.function("SetFriction", &b2Fixture::SetFriction);
	b2FixtureClass.function("GetRestitution", &b2Fixture::GetRestitution);
	b2FixtureClass.function("SetRestitution", &b2Fixture::SetRestitution);
	b2FixtureClass.function("GetAABB", &b2Fixture::GetAABB);
	b2FixtureClass.function("Dump", &b2Fixture::Dump);
	b2mod.class_("Fixture", b2FixtureClass);

	v8pp::class_<b2World> b2WorldClass(isolate);
	b2WorldClass.auto_wrap_objects(false);
	b2WorldClass.ctor<const b2Vec2&>();
	b2WorldClass.function("SetDestructionListener", &b2World::SetDestructionListener);
	b2WorldClass.function("SetContactFilter", &b2World::SetContactFilter);
	b2WorldClass.function("SetContactListener", &b2World::SetContactListener);
	b2WorldClass.function("SetDebugDraw", &b2World::SetDebugDraw);
	b2WorldClass.function("CreateBody", &b2World::CreateBody);
	b2WorldClass.function("DestroyBody", &b2World::DestroyBody);
	b2WorldClass.function("CreateJoint", &b2World::CreateJoint);
	b2WorldClass.function("DestroyJoint", &b2World::DestroyJoint);
	b2WorldClass.function("CreateParticleSystem", &b2World::CreateParticleSystem);
	b2WorldClass.function("DestroyParticleSystem", &b2World::DestroyParticleSystem);
	b2WorldClass.function("Step",
		static_cast<void( b2World::*)(float32 timeStep, int32 velocityIterations, int32 positionIterations)>(&b2World::Step));
	b2WorldClass.function("CalculateReasonableParticleIterations", &b2World::CalculateReasonableParticleIterations);
	b2WorldClass.function("ClearForces", &b2World::ClearForces);
	b2WorldClass.function("DrawDebugData", &b2World::DrawDebugData);
	b2WorldClass.function("QueryAABB", &b2World::QueryAABB);
	b2WorldClass.function("QueryShapeAABB", &b2World::QueryShapeAABB);
	b2WorldClass.function("RayCast", &b2World::RayCast);
	// TODO: b2WorldClass.function("GetBodyList", &b2World::GetBodyList);
	// TODO: b2WorldClass.function("GetJointList", &b2World::GetJointList);
	// TODO: b2WorldClass.function("GetParticleSystemList", &b2World::GetParticleSystemList);
	// TODO: b2WorldClass.function("GetContactList", &b2World::GetContactList);
	b2WorldClass.function("SetAllowSleeping", &b2World::SetAllowSleeping);
	b2WorldClass.function("GetAllowSleeping", &b2World::GetAllowSleeping);
	b2WorldClass.function("SetWarmStarting", &b2World::SetWarmStarting);
	b2WorldClass.function("GetWarmStarting", &b2World::GetWarmStarting);
	b2WorldClass.function("SetContinuousPhysics", &b2World::SetContinuousPhysics);
	b2WorldClass.function("GetContinuousPhysics", &b2World::GetContinuousPhysics);
	b2WorldClass.function("SetSubStepping", &b2World::SetSubStepping);
	b2WorldClass.function("GetSubStepping", &b2World::GetSubStepping);
	b2WorldClass.function("GetProxyCount", &b2World::GetProxyCount);
	b2WorldClass.function("GetBodyCount", &b2World::GetBodyCount);
	b2WorldClass.function("GetJointCount", &b2World::GetJointCount);
	b2WorldClass.function("GetContactCount", &b2World::GetContactCount);
	b2WorldClass.function("GetTreeHeight", &b2World::GetTreeHeight);
	b2WorldClass.function("GetTreeBalance", &b2World::GetTreeBalance);
	b2WorldClass.function("GetTreeQuality", &b2World::GetTreeQuality);
	b2WorldClass.function("SetGravity", &b2World::SetGravity);
	b2WorldClass.function("GetGravity", &b2World::GetGravity);
	b2WorldClass.function("IsLocked", &b2World::IsLocked);
	b2WorldClass.function("SetAutoClearForces", &b2World::SetAutoClearForces);
	b2WorldClass.function("GetAutoClearForces", &b2World::GetAutoClearForces);
	b2WorldClass.function("ShiftOrigin", &b2World::ShiftOrigin);
	b2WorldClass.function("GetContactManager", &b2World::GetContactManager);
	b2WorldClass.function("GetProfile", &b2World::GetProfile);
	b2WorldClass.function("Dump", &b2World::Dump);
	b2WorldClass.function("GetVersion", &b2World::GetVersion);
	b2WorldClass.function("GetVersionString", &b2World::GetVersionString);
	b2mod.class_("World", b2WorldClass);

	// b2Contact.h
	b2mod.function("MixFriction", b2MixFriction);
	b2mod.function("MixRestitution", b2MixRestitution);

	v8pp::class_<b2ContactEdge> b2ContactEdgeClass(isolate);
	b2ContactEdgeClass.auto_wrap_objects();
	b2ContactEdgeClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			return new b2ContactEdge();
		});
	b2ContactEdgeClass.var("other", &b2ContactEdge::other);
	b2ContactEdgeClass.var("contact", &b2ContactEdge::contact);
	b2ContactEdgeClass.var("prev", &b2ContactEdge::prev);
	b2ContactEdgeClass.var("next", &b2ContactEdge::next);
	b2mod.class_("ContactEdge", b2ContactEdgeClass);

	v8pp::class_<b2Contact> b2ContactClass(isolate, [](v8::Isolate* isolate, b2Contact* contact)
	{
		// cannot manually destroy or construct contacts.
	});
	b2ContactClass.auto_wrap_objects();
	b2ContactClass.function("GetManifold", static_cast<b2Manifold* (b2Contact::*)()>(&b2Contact::GetManifold));
	b2ContactClass.function("GetWorldManifold", &b2Contact::GetWorldManifold);
	b2ContactClass.function("IsTouching", &b2Contact::IsTouching);
	b2ContactClass.function("SetEnabled", &b2Contact::SetEnabled);
	b2ContactClass.function("IsEnabled", &b2Contact::IsEnabled);
	b2ContactClass.function("GetNext", static_cast<b2Contact*( b2Contact::*)()>(&b2Contact::GetNext));
	b2ContactClass.function("GetFixtureA", static_cast<b2Fixture*( b2Contact::*)()>(&b2Contact::GetFixtureA));
	b2ContactClass.function("GetFixtureB", static_cast<b2Fixture*( b2Contact::*)()>(&b2Contact::GetFixtureB));
	b2ContactClass.function("GetChildIndexB",&b2Contact::GetChildIndexB);
	b2ContactClass.function("SetFriction",&b2Contact::SetFriction);
	b2ContactClass.function("GetFriction",&b2Contact::GetFriction);
	b2ContactClass.function("ResetFriction",&b2Contact::ResetFriction);
	b2ContactClass.function("SetRestitution",&b2Contact::SetRestitution);
	b2ContactClass.function("GetRestitution",&b2Contact::GetRestitution);
	b2ContactClass.function("ResetRestitution",&b2Contact::ResetRestitution);
	b2ContactClass.function("SetTangentSpeed",&b2Contact::SetTangentSpeed);
	b2ContactClass.function("GetTangentSpeed",&b2Contact::GetTangentSpeed);
	b2ContactClass.function("GetTangentSpeed",&b2Contact::GetTangentSpeed);
	b2mod.class_("Contact", b2ContactClass);

	// b2Joint.h
	v8pp::module b2JointTypeMod(isolate);
	b2JointTypeMod.const_("unknownJoint", b2JointType::e_unknownJoint);
	b2JointTypeMod.const_("revoluteJoint", b2JointType::e_revoluteJoint);
	b2JointTypeMod.const_("prismaticJoint", b2JointType::e_prismaticJoint);
	b2JointTypeMod.const_("distanceJoint", b2JointType::e_distanceJoint);
	b2JointTypeMod.const_("pulleyJoint", b2JointType::e_pulleyJoint);
	b2JointTypeMod.const_("mouseJoint", b2JointType::e_mouseJoint);
	b2JointTypeMod.const_("gearJoint", b2JointType::e_gearJoint);
	b2JointTypeMod.const_("wheelJoint", b2JointType::e_wheelJoint);
	b2JointTypeMod.const_("weldJoint", b2JointType::e_weldJoint);
	b2JointTypeMod.const_("frictionJoint", b2JointType::e_frictionJoint);
	b2JointTypeMod.const_("ropeJoint", b2JointType::e_ropeJoint);
	b2JointTypeMod.const_("motorJoint", b2JointType::e_motorJoint);
	b2mod.submodule("JointType", b2JointTypeMod);

	v8pp::module b2LimitStateMod(isolate);
	b2LimitStateMod.const_("inactiveLimit", b2LimitState::e_inactiveLimit);
	b2LimitStateMod.const_("atLowerLimit", b2LimitState::e_atLowerLimit);
	b2LimitStateMod.const_("atUpperLimit", b2LimitState::e_atUpperLimit);
	b2LimitStateMod.const_("equalLimits", b2LimitState::e_equalLimits);
	b2mod.submodule("LimitState", b2LimitStateMod);

	v8pp::class_<b2Jacobian> b2JacobianClass(isolate);
	b2JacobianClass.auto_wrap_objects();
	b2JacobianClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			return new b2Jacobian();
		});
	b2JacobianClass.var("linear", &b2Jacobian::linear);
	b2JacobianClass.var("angularA", &b2Jacobian::angularA);
	b2JacobianClass.var("angularB", &b2Jacobian::angularB);
	b2mod.class_("Jacobian", b2JacobianClass);

	v8pp::class_<b2JointEdge> b2JointEdgeClass(isolate);
	b2JointEdgeClass.auto_wrap_objects();
	b2JointEdgeClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			return new b2JointEdge();
		});
	b2JointEdgeClass.var("other", &b2JointEdge::other);
	b2JointEdgeClass.var("joint", &b2JointEdge::joint);
	b2JointEdgeClass.var("prev", &b2JointEdge::prev);
	b2JointEdgeClass.var("next", &b2JointEdge::next);
	b2mod.class_("JointEdge", b2JointEdgeClass);

	v8pp::class_<b2JointDef> b2JointDefClass(isolate);
	b2JointDefClass.ctor<>();
	b2JointDefClass.var("type", &b2JointDef::type);
	b2JointDefClass.var("bodyA", &b2JointDef::bodyA);
	b2JointDefClass.var("bodyB", &b2JointDef::bodyB);
	b2JointDefClass.var("collideConnected", &b2JointDef::collideConnected);
	// TODO: b2JointDefClass.var("userData", &b2JointDef::userData);
	b2mod.class_("JointDef", b2JointDefClass);

	v8pp::class_<b2Joint> b2JointClass(isolate, [](v8::Isolate* isolate, b2Joint* joint)
	{
		// cannot create or destroy joint directly
	});
	b2JointClass.function("GetType", &b2Joint::GetType);
	b2JointClass.function("GetBodyA", &b2Joint::GetBodyA);
	b2JointClass.function("GetBodyB", &b2Joint::GetBodyB);
	b2JointClass.function("GetAnchorA", &b2Joint::GetAnchorA);
	b2JointClass.function("GetAnchorB", &b2Joint::GetAnchorB);
	b2JointClass.function("GetReactionForce", &b2Joint::GetReactionForce);
	b2JointClass.function("GetReactionTorque", &b2Joint::GetReactionTorque);
	b2JointClass.function("GetNext", static_cast<b2Joint*( b2Joint::*)()>(&b2Joint::GetNext));
	// TODO: b2JointClass.function("GetUserData", &b2Joint::GetUserData);
	// TODO: b2JointClass.function("SetUserData", &b2Joint::SetUserData);
	b2JointClass.function("IsActive", &b2Joint::IsActive);
	b2JointClass.function("GetCollideConnected", &b2Joint::GetCollideConnected);
	b2JointClass.function("Dump", &b2Joint::Dump);
	b2JointClass.function("ShiftOrigin", &b2Joint::ShiftOrigin);
	b2mod.class_("Joint", b2JointClass);

	v8pp::class_<b2DistanceJointDef> b2DistanceJointDefClass(isolate);
	b2DistanceJointDefClass.inherit<b2JointDef>();
	b2DistanceJointDefClass.ctor<>();
	b2DistanceJointDefClass.function("Initialize", &b2DistanceJointDef::Initialize);
	b2DistanceJointDefClass.var("localAnchorA", &b2DistanceJointDef::localAnchorA);
	b2DistanceJointDefClass.var("localAnchorB", &b2DistanceJointDef::localAnchorB);
	b2DistanceJointDefClass.var("length", &b2DistanceJointDef::length);
	b2DistanceJointDefClass.var("frequencyHz", &b2DistanceJointDef::frequencyHz);
	b2DistanceJointDefClass.var("dampingRatio", &b2DistanceJointDef::dampingRatio);
	b2mod.class_("DistanceJointDef", b2DistanceJointDefClass);

	v8pp::class_<b2DistanceJoint> b2DistanceJointClass(isolate);
	b2DistanceJointClass.inherit<b2Joint>();
	b2DistanceJointClass.function("GetLocalAnchorA", &b2DistanceJoint::GetLocalAnchorA);
	b2DistanceJointClass.function("GetLocalAnchorB", &b2DistanceJoint::GetLocalAnchorB);
	b2DistanceJointClass.function("SetLength", &b2DistanceJoint::SetLength);
	b2DistanceJointClass.function("GetLength", &b2DistanceJoint::GetLength);
	b2DistanceJointClass.function("SetFrequency", &b2DistanceJoint::SetFrequency);
	b2DistanceJointClass.function("GetFrequency", &b2DistanceJoint::GetFrequency);
	b2DistanceJointClass.function("SetDampingRatio", &b2DistanceJoint::SetDampingRatio);
	b2DistanceJointClass.function("GetDampingRatio", &b2DistanceJoint::GetDampingRatio);
	b2mod.class_("DistanceJoint", b2DistanceJointClass);

	v8pp::class_<b2FrictionJointDef> b2FrictionJointDefClass(isolate);
	b2FrictionJointDefClass.inherit<b2JointDef>();
	b2FrictionJointDefClass.ctor<>();
	b2FrictionJointDefClass.function("Initialize", &b2FrictionJointDef::Initialize);
	b2FrictionJointDefClass.var("localAnchorA", &b2FrictionJointDef::localAnchorA);
	b2FrictionJointDefClass.var("localAnchorB", &b2FrictionJointDef::localAnchorB);
	b2FrictionJointDefClass.var("maxForce", &b2FrictionJointDef::maxForce);
	b2FrictionJointDefClass.var("maxTorque", &b2FrictionJointDef::maxTorque);
	b2mod.class_("FrictionJointDef", b2FrictionJointDefClass);


	v8pp::class_<b2FrictionJoint> b2FrictionJointClass(isolate);
	b2FrictionJointClass.inherit<b2Joint>();
	b2FrictionJointClass.function("GetLocalAnchorA", &b2FrictionJoint::GetLocalAnchorA);
	b2FrictionJointClass.function("GetLocalAnchorB", &b2FrictionJoint::GetLocalAnchorB);
	b2FrictionJointClass.function("SetMaxForce", &b2FrictionJoint::SetMaxForce);
	b2FrictionJointClass.function("GetMaxForce", &b2FrictionJoint::GetMaxForce);
	b2FrictionJointClass.function("SetMaxTorque", &b2FrictionJoint::SetMaxTorque);
	b2FrictionJointClass.function("GetMaxTorque", &b2FrictionJoint::GetMaxTorque);
	b2mod.class_("FrictionJoint", b2FrictionJointClass);

	v8pp::class_<b2GearJointDef> b2GearJointDefClass(isolate);
	b2GearJointDefClass.inherit<b2JointDef>();
	b2GearJointDefClass.ctor<>();
	b2GearJointDefClass.var("joint1", &b2GearJointDef::joint1);
	b2GearJointDefClass.var("joint2", &b2GearJointDef::joint2);
	b2GearJointDefClass.var("ratio", &b2GearJointDef::ratio);
	b2mod.class_("GearJointDef", b2GearJointDefClass);

	v8pp::class_<b2GearJoint> b2GearJointClass(isolate);
	b2GearJointClass.inherit<b2Joint>();
	b2GearJointClass.function("GetJoint1", &b2GearJoint::GetJoint1);
	b2GearJointClass.function("GetJoint2", &b2GearJoint::GetJoint2);
	b2GearJointClass.function("SetRatio", &b2GearJoint::SetRatio);
	b2GearJointClass.function("GetRatio", &b2GearJoint::GetRatio);
	b2mod.class_("GearJoint", b2GearJointClass);

	v8pp::class_<b2MotorJointDef> b2MotorJointDefClass(isolate);
	b2MotorJointDefClass.inherit<b2JointDef>();
	b2MotorJointDefClass.ctor<>();
	b2MotorJointDefClass.function("Initialize", &b2MotorJointDef::Initialize);
	b2MotorJointDefClass.var("linearOffset", &b2MotorJointDef::linearOffset);
	b2MotorJointDefClass.var("angularOffset", &b2MotorJointDef::angularOffset);
	b2MotorJointDefClass.var("maxForce", &b2MotorJointDef::maxForce);
	b2MotorJointDefClass.var("maxTorque", &b2MotorJointDef::maxTorque);
	b2MotorJointDefClass.var("correctionFactor", &b2MotorJointDef::correctionFactor);
	b2mod.class_("MotorJointDef", b2MotorJointDefClass);

	v8pp::class_<b2MotorJoint> b2MotorJointClass(isolate);
	b2MotorJointClass.inherit<b2Joint>();
	b2MotorJointClass.function("SetLinearOffset", &b2MotorJoint::SetLinearOffset);
	b2MotorJointClass.function("GetLinearOffset", &b2MotorJoint::GetLinearOffset);
	b2MotorJointClass.function("SetAngularOffset", &b2MotorJoint::SetAngularOffset);
	b2MotorJointClass.function("GetAngularOffset", &b2MotorJoint::GetAngularOffset);
	b2MotorJointClass.function("SetMaxForce", &b2MotorJoint::SetMaxForce);
	b2MotorJointClass.function("GetMaxForce", &b2MotorJoint::GetMaxForce);
	b2MotorJointClass.function("SetMaxTorque", &b2MotorJoint::SetMaxTorque);
	b2MotorJointClass.function("GetMaxTorque", &b2MotorJoint::GetMaxTorque);
	b2MotorJointClass.function("SetCorrectionFactor", &b2MotorJoint::SetCorrectionFactor);
	b2MotorJointClass.function("GetCorrectionFactor", &b2MotorJoint::GetCorrectionFactor);
	b2mod.class_("MotorJoint", b2MotorJointClass);

	v8pp::class_<b2MouseJointDef> b2MouseJointDefClass(isolate);
	b2MouseJointDefClass.inherit<b2JointDef>();
	b2MouseJointDefClass.ctor<>();
	b2MouseJointDefClass.var("target", &b2MouseJointDef::target);
	b2MouseJointDefClass.var("maxForce", &b2MouseJointDef::maxForce);
	b2MouseJointDefClass.var("frequencyHz", &b2MouseJointDef::frequencyHz);
	b2MouseJointDefClass.var("dampingRatio", &b2MouseJointDef::dampingRatio);
	b2mod.class_("MouseJointDef", b2MouseJointDefClass);

	v8pp::class_<b2MouseJoint> b2MouseJointClass(isolate);
	b2MouseJointClass.inherit<b2Joint>();
	b2MouseJointClass.function("SetTarget", &b2MouseJoint::SetTarget);
	b2MouseJointClass.function("GetTarget", &b2MouseJoint::GetTarget);
	b2MouseJointClass.function("SetMaxForce", &b2MouseJoint::SetMaxForce);
	b2MouseJointClass.function("GetMaxForce", &b2MouseJoint::GetMaxForce);
	b2MouseJointClass.function("SetFrequency", &b2MouseJoint::SetFrequency);
	b2MouseJointClass.function("GetFrequency", &b2MouseJoint::GetFrequency);
	b2MouseJointClass.function("SetDampingRatio", &b2MouseJoint::SetDampingRatio);
	b2MouseJointClass.function("GetDampingRatio", &b2MouseJoint::GetDampingRatio);
	b2mod.class_("MouseJoint", b2MouseJointClass);

	v8pp::class_<b2PrismaticJointDef> b2PrismaticJointDefClass(isolate);
	b2PrismaticJointDefClass.inherit<b2JointDef>();
	b2PrismaticJointDefClass.ctor<>();
	b2PrismaticJointDefClass.function("Initialize", &b2PrismaticJointDef::Initialize);
	b2PrismaticJointDefClass.var("localAnchorA", &b2PrismaticJointDef::localAnchorA);
	b2PrismaticJointDefClass.var("localAnchorB", &b2PrismaticJointDef::localAnchorB);
	b2PrismaticJointDefClass.var("localAxisA", &b2PrismaticJointDef::localAxisA);
	b2PrismaticJointDefClass.var("referenceAngle", &b2PrismaticJointDef::referenceAngle);
	b2PrismaticJointDefClass.var("enableLimit", &b2PrismaticJointDef::enableLimit);
	b2PrismaticJointDefClass.var("lowerTranslation", &b2PrismaticJointDef::lowerTranslation);
	b2PrismaticJointDefClass.var("upperTranslation", &b2PrismaticJointDef::upperTranslation);
	b2PrismaticJointDefClass.var("enableMotor", &b2PrismaticJointDef::enableMotor);
	b2PrismaticJointDefClass.var("maxMotorForce", &b2PrismaticJointDef::maxMotorForce);
	b2PrismaticJointDefClass.var("motorSpeed", &b2PrismaticJointDef::motorSpeed);
	b2mod.class_("PrismaticJointDef", b2PrismaticJointDefClass);

	v8pp::class_<b2PrismaticJoint> b2PrismaticJointClass(isolate);
	b2PrismaticJointClass.inherit<b2Joint>();
	b2PrismaticJointClass.function("GetLocalAnchorA", &b2PrismaticJoint::GetLocalAnchorA);
	b2PrismaticJointClass.function("GetLocalAnchorB", &b2PrismaticJoint::GetLocalAnchorB);
	b2PrismaticJointClass.function("GetLocalAxisA", &b2PrismaticJoint::GetLocalAxisA);
	b2PrismaticJointClass.function("GetReferenceAngle", &b2PrismaticJoint::GetReferenceAngle);
	b2PrismaticJointClass.function("GetJointTranslation", &b2PrismaticJoint::GetJointTranslation);
	b2PrismaticJointClass.function("GetJointSpeed", &b2PrismaticJoint::GetJointSpeed);
	b2PrismaticJointClass.function("IsLimitEnabled", &b2PrismaticJoint::IsLimitEnabled);
	b2PrismaticJointClass.function("EnableLimit", &b2PrismaticJoint::EnableLimit);
	b2PrismaticJointClass.function("GetLowerLimit", &b2PrismaticJoint::GetLowerLimit);
	b2PrismaticJointClass.function("GetUpperLimit", &b2PrismaticJoint::GetUpperLimit);
	b2PrismaticJointClass.function("SetLimits", &b2PrismaticJoint::SetLimits);
	b2PrismaticJointClass.function("IsMotorEnabled", &b2PrismaticJoint::IsMotorEnabled);
	b2PrismaticJointClass.function("EnableMotor", &b2PrismaticJoint::EnableMotor);
	b2PrismaticJointClass.function("SetMotorSpeed", &b2PrismaticJoint::SetMotorSpeed);
	b2PrismaticJointClass.function("GetMotorSpeed", &b2PrismaticJoint::GetMotorSpeed);
	b2PrismaticJointClass.function("SetMaxMotorForce", &b2PrismaticJoint::SetMaxMotorForce);
	b2PrismaticJointClass.function("GetMaxMotorForce", &b2PrismaticJoint::GetMaxMotorForce);
	b2PrismaticJointClass.function("GetMotorForce", &b2PrismaticJoint::GetMotorForce);
	b2mod.class_("PrismaticJoint", b2PrismaticJointClass);


	v8pp::class_<b2PulleyJointDef> b2PulleyJointDefClass(isolate);
	b2PulleyJointDefClass.inherit<b2JointDef>();
	b2PulleyJointDefClass.ctor<>();
	b2PulleyJointDefClass.function("Initialize", &b2PulleyJointDef::Initialize);
	b2PulleyJointDefClass.var("groundAnchorA", &b2PulleyJointDef::groundAnchorA);
	b2PulleyJointDefClass.var("groundAnchorB", &b2PulleyJointDef::groundAnchorB);
	b2PulleyJointDefClass.var("localAnchorA", &b2PulleyJointDef::localAnchorA);
	b2PulleyJointDefClass.var("localAnchorB", &b2PulleyJointDef::localAnchorB);
	b2PulleyJointDefClass.var("lengthA", &b2PulleyJointDef::lengthA);
	b2PulleyJointDefClass.var("lengthB", &b2PulleyJointDef::lengthB);
	b2PulleyJointDefClass.var("ratio", &b2PulleyJointDef::ratio);
	b2mod.class_("PulleyJointDef", b2PulleyJointDefClass);

	v8pp::class_<b2PulleyJoint> b2PulleyJointClass(isolate);
	b2PulleyJointClass.inherit<b2Joint>();
	b2PulleyJointClass.function("GetGroundAnchorA", &b2PulleyJoint::GetGroundAnchorA);
	b2PulleyJointClass.function("GetGroundAnchorB", &b2PulleyJoint::GetGroundAnchorB);
	b2PulleyJointClass.function("GetLengthA", &b2PulleyJoint::GetLengthA);
	b2PulleyJointClass.function("GetLengthB", &b2PulleyJoint::GetLengthB);
	b2PulleyJointClass.function("GetRatio", &b2PulleyJoint::GetRatio);
	b2PulleyJointClass.function("GetCurrentLengthA", &b2PulleyJoint::GetCurrentLengthA);
	b2PulleyJointClass.function("GetCurrentLengthB", &b2PulleyJoint::GetCurrentLengthB);
	b2mod.class_("PulleyJoint", b2PulleyJointClass);


	v8pp::class_<b2RevoluteJointDef> b2RevoluteJointDefClass(isolate);
	b2RevoluteJointDefClass.inherit<b2JointDef>();
	b2RevoluteJointDefClass.ctor<>();
	b2RevoluteJointDefClass.function("Initialize", &b2RevoluteJointDef::Initialize);
	b2RevoluteJointDefClass.var("localAnchorA", &b2RevoluteJointDef::localAnchorA);
	b2RevoluteJointDefClass.var("localAnchorB", &b2RevoluteJointDef::localAnchorB);
	b2RevoluteJointDefClass.var("referenceAngle", &b2RevoluteJointDef::referenceAngle);
	b2RevoluteJointDefClass.var("enableLimit", &b2RevoluteJointDef::enableLimit);
	b2RevoluteJointDefClass.var("lowerAngle", &b2RevoluteJointDef::lowerAngle);
	b2RevoluteJointDefClass.var("upperAngle", &b2RevoluteJointDef::upperAngle);
	b2RevoluteJointDefClass.var("enableMotor", &b2RevoluteJointDef::enableMotor);
	b2RevoluteJointDefClass.var("motorSpeed", &b2RevoluteJointDef::motorSpeed);
	b2RevoluteJointDefClass.var("maxMotorTorque", &b2RevoluteJointDef::maxMotorTorque);
	b2mod.class_("RevoluteJointDef", b2RevoluteJointDefClass);


	v8pp::class_<b2RevoluteJoint> b2RevoluteJointClass(isolate);
	b2RevoluteJointClass.inherit<b2Joint>();
	b2RevoluteJointClass.function("GetLocalAnchorA", &b2RevoluteJoint::GetLocalAnchorA);
	b2RevoluteJointClass.function("GetLocalAnchorB", &b2RevoluteJoint::GetLocalAnchorB);
	b2RevoluteJointClass.function("GetReferenceAngle", &b2RevoluteJoint::GetReferenceAngle);
	b2RevoluteJointClass.function("GetJointAngle", &b2RevoluteJoint::GetJointAngle);
	b2RevoluteJointClass.function("GetJointSpeed", &b2RevoluteJoint::GetJointSpeed);
	b2RevoluteJointClass.function("IsLimitEnabled", &b2RevoluteJoint::IsLimitEnabled);
	b2RevoluteJointClass.function("EnableLimit", &b2RevoluteJoint::EnableLimit);
	b2RevoluteJointClass.function("GetLowerLimit", &b2RevoluteJoint::GetLowerLimit);
	b2RevoluteJointClass.function("GetUpperLimit", &b2RevoluteJoint::GetUpperLimit);
	b2RevoluteJointClass.function("SetLimits", &b2RevoluteJoint::SetLimits);
	b2RevoluteJointClass.function("IsMotorEnabled", &b2RevoluteJoint::IsMotorEnabled);
	b2RevoluteJointClass.function("EnableMotor", &b2RevoluteJoint::EnableMotor);
	b2RevoluteJointClass.function("SetMotorSpeed", &b2RevoluteJoint::SetMotorSpeed);
	b2RevoluteJointClass.function("GetMotorSpeed", &b2RevoluteJoint::GetMotorSpeed);
	b2RevoluteJointClass.function("SetMaxMotorTorque", &b2RevoluteJoint::SetMaxMotorTorque);
	b2RevoluteJointClass.function("GetMaxMotorTorque", &b2RevoluteJoint::GetMaxMotorTorque);
	b2RevoluteJointClass.function("GetReactionForce", &b2RevoluteJoint::GetReactionForce);
	b2RevoluteJointClass.function("GetReactionTorque", &b2RevoluteJoint::GetReactionTorque);
	b2RevoluteJointClass.function("GetMotorTorque", &b2RevoluteJoint::GetMotorTorque);
	b2mod.class_("RevoluteJoint", b2RevoluteJointClass);


	v8pp::class_<b2RopeJointDef> b2RopeJointDefClass(isolate);
	b2RopeJointDefClass.inherit<b2JointDef>();
	b2RopeJointDefClass.ctor<>();
	b2RopeJointDefClass.var("localAnchorA", &b2RopeJointDef::localAnchorA);
	b2RopeJointDefClass.var("localAnchorB", &b2RopeJointDef::localAnchorB);
	b2RopeJointDefClass.var("maxLength", &b2RopeJointDef::maxLength);
	b2mod.class_("RopeJointDef", b2RopeJointDefClass);

	v8pp::class_<b2RopeJoint> b2RopeJointClass(isolate);
	b2RopeJointClass.inherit<b2Joint>();
	b2RopeJointClass.function("GetLocalAnchorA", &b2RopeJoint::GetLocalAnchorA);
	b2RopeJointClass.function("GetLocalAnchorB", &b2RopeJoint::GetLocalAnchorB);
	b2RopeJointClass.function("SetMaxLength", &b2RopeJoint::SetMaxLength);
	b2RopeJointClass.function("GetMaxLength", &b2RopeJoint::GetMaxLength);
	b2RopeJointClass.function("GetLimitState", &b2RopeJoint::GetLimitState);
	b2mod.class_("RopeJoint", b2RopeJointClass);


	v8pp::class_<b2WeldJointDef> b2WeldJointDefClass(isolate);
	b2WeldJointDefClass.inherit<b2JointDef>();
	b2WeldJointDefClass.ctor<>();
	b2WeldJointDefClass.function("Initialize", &b2WeldJointDef::Initialize);
	b2WeldJointDefClass.var("localAnchorA", &b2WeldJointDef::localAnchorA);
	b2WeldJointDefClass.var("localAnchorB", &b2WeldJointDef::localAnchorB);
	b2WeldJointDefClass.var("referenceAngle", &b2WeldJointDef::referenceAngle);
	b2WeldJointDefClass.var("frequencyHz", &b2WeldJointDef::frequencyHz);
	b2WeldJointDefClass.var("dampingRatio", &b2WeldJointDef::dampingRatio);
	b2mod.class_("WeldJointDef", b2WeldJointDefClass);

	v8pp::class_<b2WeldJoint> b2WeldJointClass(isolate);
	b2WeldJointClass.inherit<b2Joint>();
	b2WeldJointClass.function("GetLocalAnchorA", &b2WeldJoint::GetLocalAnchorA);
	b2WeldJointClass.function("GetLocalAnchorB", &b2WeldJoint::GetLocalAnchorB);
	b2WeldJointClass.function("GetReferenceAngle", &b2WeldJoint::GetReferenceAngle);
	b2WeldJointClass.function("SetFrequency", &b2WeldJoint::SetFrequency);
	b2WeldJointClass.function("GetFrequency", &b2WeldJoint::GetFrequency);
	b2WeldJointClass.function("SetDampingRatio", &b2WeldJoint::SetDampingRatio);
	b2WeldJointClass.function("GetDampingRatio", &b2WeldJoint::GetDampingRatio);
	b2mod.class_("WeldJoint", b2WeldJointClass);

	
	v8pp::class_<b2WheelJointDef> b2WheelJointDefClass(isolate);
	b2WheelJointDefClass.inherit<b2JointDef>();
	b2WheelJointDefClass.ctor<>();
	b2WheelJointDefClass.function("Initialize", &b2WheelJointDef::Initialize);
	b2WheelJointDefClass.var("localAnchorA", &b2WheelJointDef::localAnchorA);
	b2WheelJointDefClass.var("localAnchorB", &b2WheelJointDef::localAnchorB);
	b2WheelJointDefClass.var("localAxisA", &b2WheelJointDef::localAxisA);
	b2WheelJointDefClass.var("enableMotor", &b2WheelJointDef::enableMotor);
	b2WheelJointDefClass.var("maxMotorTorque", &b2WheelJointDef::maxMotorTorque);
	b2WheelJointDefClass.var("motorSpeed", &b2WheelJointDef::motorSpeed);
	b2WheelJointDefClass.var("frequencyHz", &b2WheelJointDef::frequencyHz);
	b2WheelJointDefClass.var("dampingRatio", &b2WheelJointDef::dampingRatio);
	b2mod.class_("WheelJointDef", b2WheelJointDefClass);

	v8pp::class_<b2WheelJoint> b2WheelJointClass(isolate);
	b2WheelJointClass.inherit<b2Joint>();
	b2WheelJointClass.function("GetLocalAnchorA", &b2WheelJoint::GetLocalAnchorA);
	b2WheelJointClass.function("GetLocalAnchorB", &b2WheelJoint::GetLocalAnchorB);
	b2WheelJointClass.function("GetLocalAxisA", &b2WheelJoint::GetLocalAxisA);
	b2WheelJointClass.function("GetJointTranslation", &b2WheelJoint::GetJointTranslation);
	b2WheelJointClass.function("GetJointSpeed", &b2WheelJoint::GetJointSpeed);
	b2WheelJointClass.function("IsMotorEnabled", &b2WheelJoint::IsMotorEnabled);
	b2WheelJointClass.function("EnableMotor", &b2WheelJoint::EnableMotor);
	b2WheelJointClass.function("SetMotorSpeed", &b2WheelJoint::SetMotorSpeed);
	b2WheelJointClass.function("GetMotorSpeed", &b2WheelJoint::GetMotorSpeed);
	b2WheelJointClass.function("SetMaxMotorTorque", &b2WheelJoint::SetMaxMotorTorque);
	b2WheelJointClass.function("GetMaxMotorTorque", &b2WheelJoint::GetMaxMotorTorque);
	b2WheelJointClass.function("GetMotorTorque", &b2WheelJoint::GetMotorTorque);
	b2WheelJointClass.function("SetSpringFrequencyHz", &b2WheelJoint::SetSpringFrequencyHz);
	b2WheelJointClass.function("GetSpringFrequencyHz", &b2WheelJoint::GetSpringFrequencyHz);
	b2WheelJointClass.function("SetSpringDampingRatio", &b2WheelJoint::SetSpringDampingRatio);
	b2WheelJointClass.function("GetSpringDampingRatio", &b2WheelJoint::GetSpringDampingRatio);
	b2mod.class_("WheelJoint", b2WheelJointClass);

	//b2Particle
	v8pp::module b2ParticleFlagModule(isolate);
	b2ParticleFlagModule.const_("waterParticle", b2ParticleFlag::b2_waterParticle);
	b2ParticleFlagModule.const_("zombieParticle", b2ParticleFlag::b2_zombieParticle);
	b2ParticleFlagModule.const_("wallParticle", b2ParticleFlag::b2_wallParticle);
	b2ParticleFlagModule.const_("springParticle", b2ParticleFlag::b2_springParticle);
	b2ParticleFlagModule.const_("elasticParticle", b2ParticleFlag::b2_elasticParticle);
	b2ParticleFlagModule.const_("viscousParticle", b2ParticleFlag::b2_viscousParticle);
	b2ParticleFlagModule.const_("powderParticle", b2ParticleFlag::b2_powderParticle);
	b2ParticleFlagModule.const_("tensileParticle", b2ParticleFlag::b2_tensileParticle);
	b2ParticleFlagModule.const_("colorMixingParticle", b2ParticleFlag::b2_colorMixingParticle);
	b2ParticleFlagModule.const_("destructionListenerParticle", b2ParticleFlag::b2_destructionListenerParticle);
	b2ParticleFlagModule.const_("barrierParticle", b2ParticleFlag::b2_barrierParticle);
	b2ParticleFlagModule.const_("staticPressureParticle", b2ParticleFlag::b2_staticPressureParticle);
	b2ParticleFlagModule.const_("reactiveParticle", b2ParticleFlag::b2_reactiveParticle);
	b2ParticleFlagModule.const_("repulsiveParticle", b2ParticleFlag::b2_repulsiveParticle);
	b2ParticleFlagModule.const_("fixtureContactListenerParticle", b2ParticleFlag::b2_fixtureContactListenerParticle);
	b2ParticleFlagModule.const_("particleContactListenerParticle", b2ParticleFlag::b2_particleContactListenerParticle);
	b2ParticleFlagModule.const_("fixtureContactFilterParticle", b2ParticleFlag::b2_fixtureContactFilterParticle);
	b2ParticleFlagModule.const_("particleContactFilterParticle", b2ParticleFlag::b2_particleContactFilterParticle);
	b2mod.submodule("ParticleFlag", b2ParticleFlagModule);

	v8pp::class_<b2ParticleColor> b2ParticleColorClass(isolate);
	b2ParticleColorClass.ctor<>([](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
			const auto isolate = args.GetIsolate();
			if (args.Length() == 0)
				return new b2ParticleColor();
			if (args.Length() == 1)
				return new b2ParticleColor(v8pp::from_v8<b2Color>(isolate, args[0]));
			return new b2ParticleColor(v8pp::from_v8<uint8>(isolate, args[0]),
				v8pp::from_v8<uint8>(isolate, args[1]),
				v8pp::from_v8<uint8>(isolate, args[2]), 
				v8pp::from_v8<uint8>(isolate, args[3]));
	});
	b2ParticleColorClass.function("IsZero", &b2ParticleColor::IsZero);
	b2ParticleColorClass.function("GetColor", &b2ParticleColor::GetColor);
	b2ParticleColorClass.function("Set", [](v8::FunctionCallbackInfo<v8::Value> const& args)
	{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<b2ParticleColor*>(isolate, args.This());
			if (args.Length() == 1)
				This->Set(v8pp::from_v8<b2Color>(isolate, args[0]));
			This->Set(v8pp::from_v8<uint8>(isolate, args[0]),
				v8pp::from_v8<uint8>(isolate, args[1]),
				v8pp::from_v8<uint8>(isolate, args[2]),
				v8pp::from_v8<uint8>(isolate, args[3]));
	});
	b2ParticleColorClass.function("Mix", &b2ParticleColor::Mix);
	b2ParticleColorClass.function("MixColors", &b2ParticleColor::MixColors);
	b2mod.class_("ParticleColor", b2ParticleColorClass);

	v8pp::class_<b2ParticleDef> b2ParticleDefClass(isolate);
	b2ParticleDefClass.ctor<>();
	b2ParticleDefClass.var("flags", &b2ParticleDef::flags);
	b2ParticleDefClass.var("position", &b2ParticleDef::position);
	b2ParticleDefClass.var("velocity", &b2ParticleDef::velocity);
	b2ParticleDefClass.var("color", &b2ParticleDef::color);
	b2ParticleDefClass.var("lifetime", &b2ParticleDef::lifetime);
	// TODO: b2ParticleDefClass.var("userData", &b2ParticleDef::userData);
	b2ParticleDefClass.var("group", &b2ParticleDef::group);
	b2mod.class_("ParticleDef", b2ParticleDefClass);

	v8pp::module b2ParticleGroupFlagModule(isolate);
	b2ParticleGroupFlagModule.const_("solidParticleGroup", b2ParticleGroupFlag::b2_solidParticleGroup);
	b2ParticleGroupFlagModule.const_("rigidParticleGroup", b2ParticleGroupFlag::b2_rigidParticleGroup);
	b2ParticleGroupFlagModule.const_("particleGroupCanBeEmpty", b2ParticleGroupFlag::b2_particleGroupCanBeEmpty);
	b2ParticleGroupFlagModule.const_("particleGroupWillBeDestroyed", b2ParticleGroupFlag::b2_particleGroupWillBeDestroyed);
	b2ParticleGroupFlagModule.const_("particleGroupNeedsUpdateDepth", b2ParticleGroupFlag::b2_particleGroupNeedsUpdateDepth);
	b2ParticleGroupFlagModule.const_("particleGroupInternalMask", b2ParticleGroupFlag::b2_particleGroupInternalMask);
	b2mod.submodule("ParticleGroupFlag", b2ParticleGroupFlagModule);

	v8pp::class_<b2ParticleGroupDef> b2ParticleGroupDefClass(isolate);
	b2ParticleGroupDefClass.ctor<>();
	b2ParticleGroupDefClass.var("flags", &b2ParticleGroupDef::flags);
	b2ParticleGroupDefClass.var("groupFlags", &b2ParticleGroupDef::groupFlags);
	b2ParticleGroupDefClass.var("position", &b2ParticleGroupDef::position);
	b2ParticleGroupDefClass.var("angle", &b2ParticleGroupDef::angle);
	b2ParticleGroupDefClass.var("linearVelocity", &b2ParticleGroupDef::linearVelocity);
	b2ParticleGroupDefClass.var("angularVelocity", &b2ParticleGroupDef::angularVelocity);
	b2ParticleGroupDefClass.var("color", &b2ParticleGroupDef::color);
	b2ParticleGroupDefClass.var("strength", &b2ParticleGroupDef::strength);
	b2ParticleGroupDefClass.var("shape", &b2ParticleGroupDef::shape);
	// TODO: array b2ParticleGroupDefClass.var("shapes", &b2ParticleGroupDef::shapes);
	b2ParticleGroupDefClass.var("shapeCount", &b2ParticleGroupDef::shapeCount);
	b2ParticleGroupDefClass.var("stride", &b2ParticleGroupDef::stride);
	b2ParticleGroupDefClass.var("particleCount", &b2ParticleGroupDef::particleCount);
	b2ParticleGroupDefClass.var("positionData", &b2ParticleGroupDef::positionData);
	b2ParticleGroupDefClass.var("positionData", &b2ParticleGroupDef::positionData);
	b2ParticleGroupDefClass.var("lifetime", &b2ParticleGroupDef::lifetime);
	// TODO: b2ParticleGroupDefClass.var("userData", &b2ParticleGroupDef::userData);
	b2ParticleGroupDefClass.var("group", &b2ParticleGroupDef::group);
	b2mod.class_("ParticleGroupDef", b2ParticleGroupDefClass);

	v8pp::class_<b2ParticleGroup> b2ParticleGroupClass(isolate, [](v8::Isolate* isolate, b2ParticleGroup* fixture)
		{
			// cannot be created or destroyed by their own.
		});
	b2ParticleGroupClass.function("GetNext", static_cast<b2ParticleGroup*( b2ParticleGroup::*)()>(&b2ParticleGroup::GetNext));
	b2ParticleGroupClass.function("GetParticleSystem", static_cast<b2ParticleSystem*( b2ParticleGroup::*)()>(&b2ParticleGroup::GetParticleSystem));
	b2ParticleGroupClass.function("GetParticleCount", &b2ParticleGroup::GetParticleCount);
	b2ParticleGroupClass.function("GetBufferIndex", &b2ParticleGroup::GetBufferIndex);
	b2ParticleGroupClass.function("ContainsParticle", &b2ParticleGroup::ContainsParticle);
	b2ParticleGroupClass.function("GetAllParticleFlags", &b2ParticleGroup::GetAllParticleFlags);
	b2ParticleGroupClass.function("GetGroupFlags", &b2ParticleGroup::GetGroupFlags);
	b2ParticleGroupClass.function("SetGroupFlags", &b2ParticleGroup::SetGroupFlags);
	b2ParticleGroupClass.function("GetMass", &b2ParticleGroup::GetMass);
	b2ParticleGroupClass.function("GetInertia", &b2ParticleGroup::GetInertia);
	b2ParticleGroupClass.function("GetCenter", &b2ParticleGroup::GetCenter);
	b2ParticleGroupClass.function("GetLinearVelocity", &b2ParticleGroup::GetLinearVelocity);
	b2ParticleGroupClass.function("GetAngularVelocity", &b2ParticleGroup::GetAngularVelocity);
	b2ParticleGroupClass.function("GetTransform", &b2ParticleGroup::GetTransform);
	b2ParticleGroupClass.function("GetPosition", &b2ParticleGroup::GetPosition);
	b2ParticleGroupClass.function("GetAngle", &b2ParticleGroup::GetAngle);
	b2ParticleGroupClass.function("GetLinearVelocityFromWorldPoint", &b2ParticleGroup::GetLinearVelocityFromWorldPoint);
	// TODO: change if using something else as user data
	b2ParticleGroupClass.function("GetUserData", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<b2ParticleGroup*>(isolate, args.This());
			return static_cast<fse::FSEObject*>(This->GetUserData());
		});
	// TODO: change if using something else as user data
	b2ParticleGroupClass.function("SetUserData", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<b2ParticleGroup*>(isolate, args.This());
			const auto obj = v8pp::from_v8<fse::FSEObject*>(isolate, args[0]);
			This->SetUserData(obj);
		});
	b2ParticleGroupClass.function("ApplyForce", &b2ParticleGroup::ApplyForce);
	b2ParticleGroupClass.function("ApplyLinearImpulse", &b2ParticleGroup::ApplyLinearImpulse);
	b2ParticleGroupClass.function("DestroyParticles", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<b2ParticleGroup*>(isolate, args.This());
			if (args.Length() == 0)
				This->DestroyParticles();
			This->DestroyParticles(v8pp::from_v8<bool>(isolate, args[0]));
		});
	b2mod.class_("ParticleGroup", b2ParticleGroupClass);


	v8pp::class_<b2ParticleSystemDef> b2ParticleSystemDefClass(isolate);
	b2ParticleSystemDefClass.ctor<>();
	b2ParticleSystemDefClass.var("strictContactCheck", &b2ParticleSystemDef::strictContactCheck);
	b2ParticleSystemDefClass.var("filter", &b2ParticleSystemDef::filter);
	b2ParticleSystemDefClass.var("density", &b2ParticleSystemDef::density);
	b2ParticleSystemDefClass.var("gravityScale", &b2ParticleSystemDef::gravityScale);
	b2ParticleSystemDefClass.var("radius", &b2ParticleSystemDef::radius);
	b2ParticleSystemDefClass.var("maxCount", &b2ParticleSystemDef::maxCount);
	b2ParticleSystemDefClass.var("pressureStrength", &b2ParticleSystemDef::pressureStrength);
	b2ParticleSystemDefClass.var("dampingStrength", &b2ParticleSystemDef::dampingStrength);
	b2ParticleSystemDefClass.var("elasticStrength", &b2ParticleSystemDef::elasticStrength);
	b2ParticleSystemDefClass.var("springStrength", &b2ParticleSystemDef::springStrength);
	b2ParticleSystemDefClass.var("viscousStrength", &b2ParticleSystemDef::viscousStrength);
	b2ParticleSystemDefClass.var("surfaceTensionPressureStrength", &b2ParticleSystemDef::surfaceTensionPressureStrength);
	b2ParticleSystemDefClass.var("surfaceTensionNormalStrength", &b2ParticleSystemDef::surfaceTensionNormalStrength);
	b2ParticleSystemDefClass.var("repulsiveStrength", &b2ParticleSystemDef::repulsiveStrength);
	b2ParticleSystemDefClass.var("powderStrength", &b2ParticleSystemDef::powderStrength);
	b2ParticleSystemDefClass.var("ejectionStrength", &b2ParticleSystemDef::ejectionStrength);
	b2ParticleSystemDefClass.var("staticPressureStrength", &b2ParticleSystemDef::staticPressureStrength);
	b2ParticleSystemDefClass.var("staticPressureRelaxation", &b2ParticleSystemDef::staticPressureRelaxation);
	b2ParticleSystemDefClass.var("staticPressureIterations", &b2ParticleSystemDef::staticPressureIterations);
	b2ParticleSystemDefClass.var("colorMixingStrength", &b2ParticleSystemDef::colorMixingStrength);
	b2ParticleSystemDefClass.var("destroyByAge", &b2ParticleSystemDef::destroyByAge);
	b2ParticleSystemDefClass.var("lifetimeGranularity", &b2ParticleSystemDef::lifetimeGranularity);	
	b2mod.class_("ParticleSystemDef", b2ParticleSystemDefClass);

	v8pp::class_<b2ParticleSystem> b2ParticleSystemClass(isolate, [](v8::Isolate* isolate, b2ParticleSystem* fixture)
		{
			// cannot be created or destroyed by their own.
		});
	b2ParticleSystemClass.function("CreateParticle", &b2ParticleSystem::CreateParticle);
	b2ParticleSystemClass.function("GetParticleHandleFromIndex", &b2ParticleSystem::GetParticleHandleFromIndex);
	b2ParticleSystemClass.function("DestroyParticle", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<b2ParticleSystem*>(isolate, args.This());
			if (args.Length() == 1)
				This->DestroyParticle(v8pp::from_v8<int>(isolate, args[0]));
			This->DestroyParticle(v8pp::from_v8<int>(isolate, args[0]), v8pp::from_v8<int>(isolate, args[1]));
		});
	b2ParticleSystemClass.function("DestroyOldestParticle", &b2ParticleSystem::DestroyOldestParticle);
	b2ParticleSystemClass.function("DestroyParticlesInShape", [](v8::FunctionCallbackInfo<v8::Value> const& args)
		{
			const auto isolate = args.GetIsolate();
			const auto This = v8pp::from_v8<b2ParticleSystem*>(isolate, args.This());
			if (args.Length() == 2)
				This->DestroyParticlesInShape(v8pp::from_v8<b2Shape>(isolate, args[0]), v8pp::from_v8<b2Transform>(isolate, args[1]));
			This->DestroyParticlesInShape(v8pp::from_v8<b2Shape>(isolate, args[0]),
				v8pp::from_v8<b2Transform>(isolate, args[1]), 
				v8pp::from_v8<bool>(isolate, args[2]));
		});
	b2ParticleSystemClass.function("CreateParticleGroup", &b2ParticleSystem::CreateParticleGroup);
	b2ParticleSystemClass.function("JoinParticleGroups", &b2ParticleSystem::JoinParticleGroups);
	b2ParticleSystemClass.function("SplitParticleGroup", &b2ParticleSystem::SplitParticleGroup);
	// TODO: b2ParticleSystemClass.function("GetParticleGroupList", &b2ParticleSystem::GetParticleGroupList);
	b2ParticleSystemClass.function("GetParticleGroupCount", &b2ParticleSystem::GetParticleGroupCount);
	b2ParticleSystemClass.function("GetParticleCount", &b2ParticleSystem::GetParticleCount);
	b2ParticleSystemClass.function("GetMaxParticleCount", &b2ParticleSystem::GetMaxParticleCount);
	b2ParticleSystemClass.function("SetMaxParticleCount", &b2ParticleSystem::SetMaxParticleCount);
	b2ParticleSystemClass.function("GetAllParticleFlags", &b2ParticleSystem::GetAllParticleFlags);
	b2ParticleSystemClass.function("GetAllGroupFlags", &b2ParticleSystem::GetAllGroupFlags);
	b2ParticleSystemClass.function("SetPaused", &b2ParticleSystem::SetPaused);
	b2ParticleSystemClass.function("GetPaused", &b2ParticleSystem::GetPaused);
	b2ParticleSystemClass.function("SetDensity", &b2ParticleSystem::SetDensity);
	b2ParticleSystemClass.function("GetDensity", &b2ParticleSystem::GetDensity);
	b2ParticleSystemClass.function("SetGravityScale", &b2ParticleSystem::SetGravityScale);
	b2ParticleSystemClass.function("GetGravityScale", &b2ParticleSystem::GetGravityScale);
	b2ParticleSystemClass.function("SetDamping", &b2ParticleSystem::SetDamping);
	b2ParticleSystemClass.function("GetDamping", &b2ParticleSystem::GetDamping);
	b2ParticleSystemClass.function("SetStaticPressureIterations", &b2ParticleSystem::SetStaticPressureIterations);
	b2ParticleSystemClass.function("GetStaticPressureIterations", &b2ParticleSystem::GetStaticPressureIterations);
	b2ParticleSystemClass.function("SetRadius", &b2ParticleSystem::SetRadius);
	b2ParticleSystemClass.function("GetRadius", &b2ParticleSystem::GetRadius);
	// TODO: b2ParticleSystemClass.function("GetPositionBuffer", &b2ParticleSystem::GetPositionBuffer);
	// TODO: b2ParticleSystemClass.function("GetVelocityBuffer", &b2ParticleSystem::GetVelocityBuffer);
	// TODO: b2ParticleSystemClass.function("GetColorBuffer", &b2ParticleSystem::GetColorBuffer);
	// TODO: b2ParticleSystemClass.function("GetGroupBuffer", &b2ParticleSystem::GetGroupBuffer);
	// TODO: b2ParticleSystemClass.function("GetWeightBuffer", &b2ParticleSystem::GetWeightBuffer);
	//TODO: b2ParticleSystemClass.function("GetUserDataBuffer", &b2ParticleSystem::GetUserDataBuffer);
	// TODO: b2ParticleSystemClass.function("GetFlagsBuffer", &b2ParticleSystem::GetFlagsBuffer);
	b2ParticleSystemClass.function("SetParticleFlags", &b2ParticleSystem::SetParticleFlags);
	b2ParticleSystemClass.function("GetParticleFlags", &b2ParticleSystem::GetParticleFlags);
	// TODO: b2ParticleSystemClass.function("SetFlagsBuffer", &b2ParticleSystem::SetFlagsBuffer);
	// TODO: b2ParticleSystemClass.function("SetPositionBuffer", &b2ParticleSystem::SetPositionBuffer);
	// TODO: b2ParticleSystemClass.function("SetVelocityBuffer", &b2ParticleSystem::SetVelocityBuffer);
	// TODO: b2ParticleSystemClass.function("SetColorBuffer", &b2ParticleSystem::SetColorBuffer);
	// TODO: b2ParticleSystemClass.function("SetUserDataBuffer", &b2ParticleSystem::SetUserDataBuffer);
	// TODO: b2ParticleSystemClass.function("GetContacts", &b2ParticleSystem::GetContacts);
	b2ParticleSystemClass.function("GetContactCount", &b2ParticleSystem::GetContactCount);
	// TODO: b2ParticleSystemClass.function("GetBodyContacts", &b2ParticleSystem::GetBodyContacts);
	b2ParticleSystemClass.function("GetBodyContactCount", &b2ParticleSystem::GetBodyContactCount);
	// TODO: b2ParticleSystemClass.function("GetPairs", &b2ParticleSystem::GetPairs);
	b2ParticleSystemClass.function("GetPairCount", &b2ParticleSystem::GetPairCount);
	// TODO: b2ParticleSystemClass.function("GetTriads", &b2ParticleSystem::GetTriads);
	b2ParticleSystemClass.function("GetTriadCount", &b2ParticleSystem::GetTriadCount);
	b2ParticleSystemClass.function("SetStuckThreshold", &b2ParticleSystem::SetStuckThreshold);
	// TODO: b2ParticleSystemClass.function("GetStuckCandidates", &b2ParticleSystem::GetStuckCandidates);
	b2ParticleSystemClass.function("GetStuckCandidateCount", &b2ParticleSystem::GetStuckCandidateCount);
	b2ParticleSystemClass.function("ComputeCollisionEnergy", &b2ParticleSystem::ComputeCollisionEnergy);
	b2ParticleSystemClass.function("SetStrictContactCheck", &b2ParticleSystem::SetStrictContactCheck);
	b2ParticleSystemClass.function("GetStrictContactCheck", &b2ParticleSystem::GetStrictContactCheck);
	b2ParticleSystemClass.function("SetParticleLifetime", &b2ParticleSystem::SetParticleLifetime);
	b2ParticleSystemClass.function("GetParticleLifetime", &b2ParticleSystem::GetParticleLifetime);
	b2ParticleSystemClass.function("SetDestructionByAge", &b2ParticleSystem::SetDestructionByAge);
	b2ParticleSystemClass.function("GetDestructionByAge", &b2ParticleSystem::GetDestructionByAge);
	// TODO: b2ParticleSystemClass.function("GetExpirationTimeBuffer", &b2ParticleSystem::GetExpirationTimeBuffer);
	b2ParticleSystemClass.function("ExpirationTimeToLifetime", &b2ParticleSystem::ExpirationTimeToLifetime);
	// TODO: b2ParticleSystemClass.function("GetIndexByExpirationTimeBuffer", &b2ParticleSystem::GetIndexByExpirationTimeBuffer);
	b2ParticleSystemClass.function("ParticleApplyLinearImpulse", &b2ParticleSystem::ParticleApplyLinearImpulse);
	b2ParticleSystemClass.function("ApplyLinearImpulse", &b2ParticleSystem::ApplyLinearImpulse);
	b2ParticleSystemClass.function("ParticleApplyForce", &b2ParticleSystem::ParticleApplyForce);
	b2ParticleSystemClass.function("ApplyForce", &b2ParticleSystem::ApplyForce);
	b2ParticleSystemClass.function("GetNext", static_cast<b2ParticleSystem*( b2ParticleSystem::*)()>(&b2ParticleSystem::GetNext));
	// TODO: b2ParticleSystemClass.function("QueryAABB", &b2ParticleSystem::QueryAABB);
	// TODO: b2ParticleSystemClass.function("QueryShapeAABB", &b2ParticleSystem::QueryShapeAABB);
	// TODO: b2ParticleSystemClass.function("RayCast", &b2ParticleSystem::RayCast);
	// TODO: b2ParticleSystemClass.function("ComputeAABB", &b2ParticleSystem::ComputeAABB);
	b2mod.class_("ParticleSystem", b2ParticleSystemClass);


	return b2mod;
}