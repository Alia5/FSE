export function objectIsNativeOnly(obj: fse.FSEObject): boolean {
    return obj.constructor.name !== 'ScriptObjectNative'
}

export function objectIsScriptObject(obj: fse.FSEObject): boolean {
    return obj.constructor.name === 'ScriptObjectNative'
}