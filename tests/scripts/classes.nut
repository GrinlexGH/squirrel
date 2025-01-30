class BaseClass {
    constructor()
    {
        ::print("Base constructor\n");
    }
}

class ChildClass extends BaseClass {
    constructor()
    {
        base.constructor();
        ::print("Child constructor\n");
    }

    member = "child class member";
}
