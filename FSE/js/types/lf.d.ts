declare namespace lf {

    function IsValid(num: number): boolean;
    function InvSqrt(num: number): number;
    function Sqrt(num: number): number;
    function Atan2(x: number, y: number): number;
    interface Vector2 {
        x: number;
        y: number;
    }

    interface Vector3 extends Vector2 {
        z: number;
    }

    function LengthSquared(vec: Vector2): number;
	function IsValidVec2(vec: Vector2): boolean;
	function Skew(vec: Vector2): Vector2;
	function Length(vec: Vector2 | Vector3): number;
	function Normalize(vec: Vector2 | Vector3): Vector2;

    class Rot {
        public constructor(rotation?: number);
        public Set(angle: number): void;
        public SetIdentity(): void;
        public GetAngle(): number;
        public GetXAxis(): Vector2;
        public GetYAxis(): Vector2;
        public s: number;
        public c: number
    }

    class Transform {
        public constructor();
        public constructor(pos: Vector2, rotation: Rot);
        public Set(pos: Vector2, rotation: Rot): void;
        public SetIdentity(): void;
        public p: Vector2;
        public q: Rot;
    }

    class Sweep {
        public constructor();
        public GetTransform(xfb: Transform, beta: number): void;
        public Advance(alpha: number): void;
        public Normalize(): void;
        public localCenter: Vector2;
        public c0: Vector2;
        public c: Vector2;
        public a0: number;
        public a: number;
        public alpha0: number;
    }

    function Dot(vec0: Vector2, vec1: Vector2): number;
    function Dot(vec0: Vector3, vec1: Vector3): number;
    function Cross2(vec0: Vector2, vec1: Vector2): number;
    function Cross3(vec0: Vector3, vec1: Vector3): number;
    function Cross(vec: Vector2, s: number): number;
    function Cross(s: number, vec: Vector2): number;


	// ... help meh!
	// TODO: some missing funs

	function NextPowerOfTwo(a: number): number;
	function IsPowerOfTwo(a: number): boolean;

    // TODO: b2CollisionStuffs


    class AABB {
        public constructor();
        public IsValid(): boolean;
        public GetCenter(): Vector2;
        public GetExtents(): Vector2;
        public GetPerimiter(): number;
        public Combine(aabb0: AABB, aabb1?: AABB): void;
        public Contains(aabb: AABB): boolean;
        public RayCast(output: any, input: any): any; // TODO:
        public lowerBound: Vector2;
        public upperBound: Vector2;
    }

    class MassData {
        public constructor();
        public mass: number;
        public center: Vector2;
        public I: number;
    }

    abstract class Shape {
        public Clone(): Shape;
        public GetType(): Shape.Type;
        public GetChildCount(): number;
        public TestPoint(xf: Transform, p: Vector2): boolean;
        public ComputeDistance(xf: Transform, p: Vector2, distance: number, normal: Vector2, childIndex: number): void;
        public RayCast(TODO: any): any; // TODO:
        public ComputeAABB(TODO: any): any; // TODO:
        public ComputeMass(massData: MassData, density: number): void;
        public m_type: Shape.Type;
        public m_radius: number;
    }

    namespace Shape {
        enum Type {
            circle = 0,
            edge,
            polygon,
            chain,
            typeCount
        }
    }

    class CircleShape extends Shape {
        public constructor();
        public GetSupport(d: Vector2): number;
        public GetSupportVertex(d: Vector2): Vector2;
        public GetVertexCount(): number;
        public GetVertex(index: number): Vector2;
        public m_p: Vector2;
    }

    class EdgeShape extends Shape {
        public constructor();
        public m_vertex0: Vector2;
        public m_vertex1: Vector2;
        public m_vertex2: Vector2;
        public m_vertex3: Vector2;
        public m_hasVertex0: boolean;
        public m_hasVertex3: boolean;
    }

    class ChainShape extends Shape {
        public constructor();
        public CreateLoop(vertices: Vector2[], count: number): void; //TODO:
        public CreateChain(vertices: Vector2[], count: number): void; //TODO:
        public SetPreVertex(vert: Vector2): void;
        public SetNextVertex(vert: Vector2): void;
        public GetChildEdge(edge: EdgeShape, index: number): void;
        public m_vertices: Vector2[];
        public m_count: number;
        public m_preVertex: Vector2;
        public m_nextVertex: Vector2;
        public m_hasPreVertex: boolean;
        public m_hasNextVertex: boolean;
    }

    class PolygonShape extends Shape {
        public constructor();
        public Set(points: Vector2[], count: number): void; // TODO: 
        public SetAsBox(hx: number, hy: number): void;
        public SetAsBox(hx: number, hy: number, center: Vector2, angle: number): void;
        public GetVertexCount(): number;
        public GetVertex(index: number): Vector2;
        public Validate(): boolean;
        public m_centroid: Vector2;
        public m_vertices: Vector2[];
        public m_count: number;
        public m_normals: Vector2[];
    }

    enum BodyType {
        staticBody,
        kinematicBody, 
        dynamicBody
    }
    class BodyDef {
        public constructor();
        public type: BodyType;
        public position: Vector2;
        public angle: number;
        public linearVelocity: Vector2;
        public angularVelocity: number;
        public linearDamping: number;
        public angularDamping: number;
        public allowSleep: boolean;
        public awake: boolean;
        public fixedRotation: boolean;
        public bullet: boolean;
        public active: boolean;
        // public userData: cunt;
        public gravityScale: number;
    }

    class Body {
        private constructor();
        public CreateFixture(fixtureDef: FixtureDef): Fixture;
        public CreateFixture(fixtureDef: FixtureDef, density: number): Fixture;
        public DestroyFixture(fixture: Fixture): void;
        public SetTransform(pos: Vector2, angle: number): void;
        public GetTransform(): Transform;
        public GetPosition(): Vector2;
        public GetAngle(): number;
        public GetWorldCenter(): Vector2;
        public GetLocalCenter(): Vector2;
        public SetLinearVelocity(velocity: Vector2): void;
        public GetLinearVelocity(): Vector2;
        public SetAngularVelocity(v: number): void;
        public GetAngularVelocity(): number;
        public ApplyForce(force: Vector2, point: Vector2, wake: boolean): void;
        public ApplyForceToCenter(force: Vector2, wake: boolean): void;
        public ApplyTorque(torque: number, wake: boolean): void;
        public ApplyLinearImpulse(impulse: Vector2, point: Vector2, wake: boolean): void;
        public ApplyAngularImpulse(impulse: number, wake:boolean): void;
        public GetMass(): number;
        public GetInertia(): number;
        public GetMassData(massData: MassData): void; // TODO: ?
        public SetMassData(massData: MassData): void;
        public ResetMassData(): void;
        public GetWorldPoint(localPoint: Vector2): Vector2;
        public GetWorldVector(localVector: Vector2): Vector2;
        public GetLocalPoint(worldPoint: Vector2): Vector2;
        public GetLocalVector(worldVector: Vector2): Vector2;
        public GetLinearVelocityFromWorldPoint(worldPoint: Vector2):  Vector2;
        public GetLinearVelocityFromLocalPoint(localPoint: Vector2):  Vector2;
        public GetLinearDamping(): number;
        public SetLinearDamping(damping: number): void;
        public GetAngularDamping(): number;
        public SetAngularDamping(damping: number): void;
        public GetGravityScale(): number;
        public SetGravityScale(scale: number): void;
        public SetType(type: BodyType): void
        public GetType(): BodyType;
        public SetBullet(bullet: boolean): void;
        public IsBullet(): boolean;
        public SetSleepingAllowed(sleep: boolean): void;
        public IsSleepingAllowed(): boolean;
        public SetAwake(awake: boolean): void;
        public IsAwake(): boolean;
        public SetActive(active: boolean): void;
        public IsActive(): boolean;
        public SetFixedRotation(fixed: boolean): void;
        public IsFixedRotation(): boolean;
        public GetNext(): Body;
        public GetUserData(): any; //TODO: 
        public SetUserData(data: any): any; //TODO: 
        public GetWorld(): any; //TODO;
        public Dump(): void;
    }


    class Filter {
        public constructor();
        public categoryBits: number;
        public maskBits: number;
        public groupIndex: number;
    }

    class FixtureDef {
        public constructor();
        public shape: Shape;
        public friction: number;
        public restitution: number;
        public density: number;
        public isSensor: boolean;
        public filter: Filter;
    }

    class FixtureProxy {
        public constructor();
        public aabb: AABB;
        public fixture: Fixture;
        public childIndex: number;
        public proxyId: number;
    }

    class Fixture {
        private constructor();
        public GetType(): Shape.Type;
        public GetShape(): Shape;
        public SetSensor(sensor: boolean): void;
        public IsSendor(): boolean;
        public SetFilterData(filter: Filter): void;
        public GetFilterData(): Filter;
        public Refilter(): void;
        public GetBody(): Body;
        public GetNext(): Fixture;
        public TestPoint(p: Vector2): boolean;
        public ComputeDistance(p: Vector2, distance: number, normal: Vector2, childIndex: number): void; // TODO: 
        public RayCast(todo: any): any; //TODO:
        public GetMassData(data: MassData): void; // TODO ?
        public SetDensity(density: number): void;
        public GetDensity(): number;
        public GetFriction(): number;
        public SetFriction(friction: number): void;
        public GetRestitution(): number;
        public SetRestitution(restitution: number): void;
        public GetAABB(): AABB;
        public Dump(): void; 
    }

    class World {
        public constructor(gravity: Vector2);
        public SetDestructionListener(listener: any): void; // TODO
        public SetContactFilter(filter: any): void; // TODO
        public SetContactListener(listener: any): void; // TODO
        public SetDebugDraw(debugDraw: any): void; // TODO
        public CreateBody(bodyDef: BodyDef): Body;
        public DestroyBody(body: Body): void
        public CreateJoint(jointDef: JointDef): Joint;
        public DestroyJoint(joint: Joint): void;
        public CreateParticleSystem(particleSystemDef: any): any; // TODO
        public DestroyParticleSystem(particleSystem: any): void; // TODO
        public Step(timeStep: number, velocityIterations: number, positionIterations: number): void;
        public ClearForces(): void;
        public DrawDebugData(): void;
        public QueryAABB(...TODO: any): void; // TODO
        public QueryShapeAABB(...TODO: any): void; // TODO
        public RayCast(...TODO: any): void; // TODO
        // TODO: GetBodyList, 
        // TODO: GetJointList, 
        // TODO: GetParticleSystemList, 
        // TODO: GetContactList, 
        public SetAllowSleeping(allow: boolean): void;
        public GetAllowSleeping(): boolean;
        public SetWarmStarting(warm: boolean): void;
        public GetWarmStarting(): boolean;
        public SetContinuousPhysics(cont: boolean): boolean;
        public GetContinuousPhysics(): boolean;
        public SetSubStepping(substep: boolean): void;
        public GetSubStepping(): boolean;
        public GetProxyCount(): number;
        public GetBodyCount(): number;
        public GetJointCount(): number;
        public GetContactCount(): number;
        public GetTreeHeight(): number;
        public GetTreeBalance(): number;
        public GetTreeQuality(): number;
        public SetGravity(gravity: Vector2): void;
        public GetGravity(): Vector2;
        public IsLocked(): boolean;
        public SetAutoClearForces(flag: boolean): void;
        public GetAutoClearForces(): boolean;
        public ShiftOrigin(newOrigin: Vector2): void;
        public GetContactManager(): any; // TODO
        public GetProfile(): any; // TODO
        public Dump(): void;
        public GetVersion(): object; //TODO
        public GetVersionString(): string;

    }

    // TODO: more contact stuffs.

    enum JointType {
        unknownJoint = 0,
        revoluteJoint, 
        prismaticJoint, 
        distanceJoint, 
        pulleyJoint, 
        mouseJoint, 
        gearJoint, 
        wheelJoint, 
        weldJoint, 
        frictionJoint, 
        ropeJoint, 
        motorJoint, 
    }

    enum LimitState {
        inactiveLimit = 0,
        atLowerLimit, 
        atUpperLimit, 
        equalLimits, 
    }

    class Jacobian {
        public constructor();
        public linear: Vector2;
        public angularA: number;
        public angularB: number;
    }

    class JointEdge {
        public constructor();
        public other: Body;
        public joint: Joint;
        public prev: JointEdge;
        public next: JointEdge;
    }

    class JointDef {
        public constructor();
        public type: JointType;
        public bodyA: Body;
        public bodyB: Body;
        public collideConnected: boolean;
    }
    abstract class Joint {
        public GetType(): JointType;
        public GetBodyA(): Body;
        public GetBodyB(): Body;
        public GetAnchorA(): Vector2;
        public GetAnchorB(): Vector2;
        public GetReactionForce(): Vector2;
        public GetReactionTourque(): number;
        public GetNext(): Joint;
        public IsActive(): boolean;
        public GetCollideConnected(): boolean;
        public Dump(): void;
        public ShiftOrigin(newOrigin: Vector2): void;
    }

    class DistanceJointDef extends JointDef {
        public constructor();
        public Initialize(bodyA: Body, bodyB: Body, anchorA: Vector2, anchorB: Vector2): void; 
        public localAnchorA: Vector2; 
        public localAnchorB: Vector2; 
        public length: number; 
        public frequencyHz: number; 
        public dampingRatio: number; 
    }

    class DistanceJoint extends Joint {
        private constructor();
        public GetLocalAnchorA(): Vector2; 
        public GetLocalAnchorB(): Vector2; 
        public SetLength(length: number): void; 
        public GetLength(): number; 
        public SetFrequency(freq: number): void; 
        public GetFrequency(): number; 
        public SetDampingRatio(ratio: number): number; 
        public GetDampingRatio(): number; 
    }

    class FrictionJointDef extends JointDef {
        public constructor();
        public Initialize(bodyA: Body, bodyB: Body, anchor: Vector2): void; 
        public localAnchorA: Vector2; 
        public localAnchorB: Vector2; 
        public maxForce: number; 
        public maxTorque: number; 
    }

    class FrictionJoint extends Joint {
        private constructor();
        public GetLocalAnchorA(): Vector2; 
        public GetLocalAnchorB(): Vector2; 
        public SetMaxForce(force: number): void; 
        public GetMaxForce(): number; 
        public SetMaxTorque(max: number): void; 
        public GetMaxTorque(): number; 
    }

    class GearJointDef extends JointDef {
        public constructor();
        public joint1: Joint; 
        public joint2: Joint; 
        public ratio: number; 
    }

    class GearJoint extends Joint {
        private constructor();
        public GetJoint1(): Joint; 
        public GetJoint2(): Joint; 
        public SetRatio(ratio: number): void; 
        public GetRatio(): number; 
    }

    class MotorJointDef extends JointDef {
        public constructor();
        public Initialize(bodyA: Body, bodyB: Body): void; 
        public linearOffset(): Vector2; 
        public angularOffset(): number; 
        public maxForce(): number; 
        public maxTorque(): number; 
        public correctionFactor(): number; 
    }

    class MotorJoint extends Joint {
        private constructor();
        public SetLinearOffset(offset: Vector2): void; 
        public GetLinearOffset(): Vector2; 
        public SetAngularOffset(offset: number): void; 
        public GetAngularOffset(): number; 
        public SetMaxForce(force: number): void; 
        public GetMaxForce(): number; 
        public SetMaxTorque(torque: number): void; 
        public GetMaxTorque(): number; 
        public SetCorrectionFactor(factor: number): void; 
        public GetCorrectionFactor(): number; 
    }

    // TODO: moreJoints

    // TODO: ParticleStuffs

}