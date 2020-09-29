import { ScriptObject } from './ScriptObject';
import { ScriptComponent } from './ScriptComponent';
import * as fseUtils from './util'
Object.assign(fse, { ScriptObject: ScriptObject, ScriptComponent: ScriptComponent, ...fseUtils });

require('./SceneExtensions');
