declare namespace sf {

    interface Vector2 {
        x: number;
        y: number;
    }

    // eslint-disable-next-line @typescript-eslint/no-empty-interface
    interface Drawable {
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

    class RenderTarget {
        public clear(): void;
        public draw(drawable: Drawable): void;
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

}