export class MouseViewController extends fse.ScriptComponent {
    private currentOffset: fse.Vector2 = {x: 0, y: 0}
    private renderTarget: sf.RenderTarget;
    public constructor() {
        super();
    }

    public onAttach() {
        this.renderTarget = this.native.getAttachedObject().getScene().getRenderTarget();
    }

    public update(deltaTime: number) {
        // if (this.mouse.isButtonPressed(MouseButtonLeft) && this.currentOffset.x == 0 && this.currentOffset.y == 0) {
        //     this.currentOffset = this.mouse.getPosition();
        // } else if (this.mouse.isButtonPressed(MouseButtonLeft)) {
        //     auto view = View(this.renderTarget.getView());
        //     auto iVec = this.currentOffset - this.mouse.getPosition();
        //     view.move(Vector2f(iVec.x, iVec.y));
        //     this.currentOffset = Vector2i(0,0);
        //     this.renderTarget.setView(view);
        // } else {
        //     this.currentOffset = Vector2i(0,0);
        // }

        if (sf.Mouse.isButtonPressed(sf.Mouse.Button.Left)) {
            if (this.currentOffset.x === 0 && this.currentOffset.y === 0) {
                this.currentOffset = sf.Mouse.getPosition();
                return;
            }
            const view = this.renderTarget.getView();
            const mousePos = sf.Mouse.getPosition();
            view.move({ 
                x: (this.currentOffset.x - mousePos.x) * 2,
                y: (this.currentOffset.y - mousePos.y) * 2
            });
            this.currentOffset = {x: 0, y: 0};
            this.renderTarget.setView(view);
        } else {
            this.currentOffset = {x: 0, y: 0};
        }
    }
}