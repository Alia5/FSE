declare namespace sf {

    interface Vector2 {
        x: number;
        y: number;
    }

    // eslint-disable-next-line @typescript-eslint/no-empty-interface
    interface Drawable {
    }

    class FloatRect {
        public constructor(rectLeft: number, rectTop: number, rectWidth: number, rectHeight: number);
        public constructor(pos: Vector2, size: Vector2);
        public contains(x: number, y: number): boolean;
        public contains(point: Vector2): boolean;
        public intersects(rect: FloatRect, intersection: FloatRect): boolean;
        public left: number;
        public top: number;
        public width: number;
        public height: number;
    }

    class IntRect {
        public constructor(rectLeft: number, rectTop: number, rectWidth: number, rectHeight: number);
        public constructor(pos: Vector2, size: Vector2);
        public contains(x: number, y: number): boolean;
        public contains(point: Vector2): boolean;
        public intersects(rect: IntRect, intersection: IntRect): boolean;
        public left: number;
        public top: number;
        public width: number;
        public height: number;
    }

    class Transformable {
        public setPosition(position: Vector2): void;
        public setPosition(x: number, y: number): void;
        public setRotation(rotation: number): void;
        public setScale(scale: Vector2): void;
        public setScale(x: number, y: number): void;
        public setOrigin(origin: Vector2): void;
        public setOrigin(x: number, y: number): void;
        public getPosition(): Vector2;
        public getRotation(): number;
        public getScale(): Vector2;
        public getOrigin(): Vector2;
        public move(x: number, y: number): void;
        public move(moveVector: Vector2): void;
        public rotate(rotate: number): void;
        public scale(scale: Vector2): void;
        public scale(x: number, y: number): void;
        public getTransform(): any;
        public getInverseTransform(): any;
    }

    class Color {
        public constructor();
        public constructor(rgba: number);
        public constructor(red: number, green: number, blue: number);
        public r: number;
        public g: number;
        public b: number;
        public toInteger(): number;
        public static Black: Color;
        public static White: Color;
        public static Red: Color;
        public static Green: Color;
        public static Blue: Color;
        public static Yellow: Color;
        public static Magenta: Color;
        public static Cyan: Color;
        public static Transparent: Color;
    }

    class View {
        public constructor();
        public constructor(rect: any);
        public constructor(pos: Vector2, size: Vector2);
        public setCenter(pos: Vector2): void;
        public setCenter(x: number, y: number): void;
        public setSize(pos: Vector2): void;
        public setSize(x: number, y: number): void;
        public setRotation(rotation: number): void;
        public setViewport(viewport: any): void;
        public reset(viewport: any): void;
        public getCenter(): Vector2;
        public getSize(): Vector2;
        public getRotation(): number;
        public getViewport(): object;
        public move(pos: Vector2): void;
        public move(x: number, y: number): void;
        public rotate(rot: number): void;
        public zoom(factor: number): void;
        public getTransform(): any;
        public getInverseTransform(): any;
    }

    class RenderTarget {
        public clear(): void;
        public draw(drawable: Drawable): void;
        public getView(): View;
        public setView(view: View): void;
    }

    class Window extends RenderTarget {
        public close(): void;
        public isOpen(): boolean;
    }

    abstract class Shape extends Transformable implements Drawable {
        public setTexture(texture: any): void;
        public setTextureRect(textureRect: any): void;
        public setFillColor(color: Color): void;
        public setOutlineColor(color: Color): void;
        public setOutlineThickness(thickness: number): void;
        public getTexture(): any;
        public getTextureRect(): any;
        public getFillColor(): Color;
        public getOutlineColor(): Color;
        public getOutlineThickness(): number;
        public getPointCount(): number;
        public getPoint(): Vector2;
        public getLocalBounds(): any;
        public getGlobalBounds(): any;
    }

    class CircleShape extends Shape {
        public constructor();
        public setRadius(radius: number): void;
        public getRadius(): number;
        public setPointCount(count: number): void;
        public getPointCount(): number;
    }

    class RectangleShape extends Shape {
        public constructor();
        public setSize(size: Vector2): void;
        public getSize(): Vector2;
        public getPointCount(): number;
    }

    namespace Mouse {
        enum Button {
            Left,
            Right,
            Middle,
            XButton1,
            XButton2,
            ButtonCount
        }
        enum Wheel {
            VerticalWheel,
            HorizontalWheel
        }
        function isButtonPressed(button: Button): boolean;
        function getPosition(window?: Window): Vector2;
        function setPosition(pos: Vector2, window?: Window): void;
    }

}