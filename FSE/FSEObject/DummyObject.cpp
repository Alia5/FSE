#include "DummyObject.h"

DummyObject::DummyObject(fse::Scene *scene) : fse::FSEObject(scene)
{
	shape = sf::CircleShape(100.f);
	shape.setFillColor(sf::Color::Green);

	m_Drawable = &shape;


	//eventCookie = scene->getEventBus()->subscribeEvent("TEST", std::bind(&DummyObject::testEventCallback, this));

}

DummyObject::~DummyObject()
{
	std::wcout << "Destroying Dummy id: " << getID() << std::endl;
}

void freefun(std::wstring s, int a)
{
	std::wcout << L"freeFun " << s << a << "\n";
}

void DummyObject::update(float deltaTime)
{
	shape.setPosition(sf::Vector2f(125.f*id, 125.f*id));

	totalTime += deltaTime;
	if (!done && id == 1 && totalTime > 3.f)
	{

		done = true;
		getSceneFSEObjects()->at(3)->destroy();


		std::unique_ptr<DummyObject> test(new DummyObject(getScene()));
		test->makeBlue();
		test->setID(5);
		getScene()->spawnFSEObject(std::move(test));


		auto tConnection = sig.connect([](std::wstring s, int i) {
		
			std::wcout <<"From tConnection\n";
			std::wcout << s << i << "\n";
			return i;
		
		});


		//sig.Connect(std::bind(&DummyObject::test, this, std::placeholders::_1, std::placeholders::_2));
		//sig.Stuff(std::bind(&DummyObject::test, this));
		//sig.Connect(sig.Bind(this, &DummyObject::test));


		sig.connect(this, &DummyObject::test);

		//connection_ = sig.Connect(std::bind(&DummyObject::test, this, std::placeholders::_1, std::placeholders::_2));

		auto c = Signal::ScopedConnection(sig, sig.connect(freefun));

		sig(L"sdsad", 23);


		bool testi = sig.disconnect(tConnection);
		std::wcout << testi << "\n";


	} else if (done && id == 1) {
		destroy();

		sig(L"ggg45t45dsdfsdsdfad", 32424234);

	}


	if (blue)
	{
		//getScene()->moveView(sf::Vector2f(10 * deltaTime, 10 * deltaTime));
	}

}




void DummyObject::draw(sf::RenderTarget& target)
{	
	target.draw(*m_Drawable);
}

void DummyObject::spawned()
{
	std::wcout << "Dummy Spawned id: " << getID() << std::endl;
}

bool DummyObject::destroy()
{


	return FSEObject::destroy();
}

void DummyObject::setID(int id)
{
	this->id = id;
	setZOrder(0);

	if (id == 1)
	{
		shape.setFillColor(sf::Color::Red);
		setZOrder(5);
	}
}

void DummyObject::makeBlue()
{
	shape.setFillColor(sf::Color::Blue);
	blue = true;
}

void DummyObject::testEventCallback()
{

	std::wcout << "TestEventCallback from Dummy id: " << getID() << std::endl;

	//std::wcout << "Callback fired from FSEObject id: " << object->getID() << std::endl;

}
