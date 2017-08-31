package de.flatspotsoftware.fse_test;

/**
 * Created by Alia5 on 8/30/2017.
 */

public class CPPCallIns {

    static {
        System.loadLibrary("fsetest-lib");
    }

    private static CPPCallIns callIns = null;

    private CPPCallIns()
    {}

    public static CPPCallIns getInstance()
    {
        if (callIns == null)
            callIns = new CPPCallIns();

        return callIns;
    }

    public native String testCall(String arg);

}
