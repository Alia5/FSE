Object.assign(fse.Scene, { constructScriptObject: (name: string, ...args: unknown[]): fse.ScriptObjectNative => {     // TODO
    const ctor = require(`./${name}`)[name] as (new () => fse.ScriptObject); // this will need some work;
    return new ctor().native;
}});
