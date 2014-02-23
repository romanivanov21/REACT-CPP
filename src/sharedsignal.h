/**
 *  SharedSignal.h
 *
 *  Signal watcher that is managed by the loop, and that can be shared with the 
 *  outside world.
 *
 *  @copyright 2014 Copernica BV
 */

/**
 *  Set up namespace
 */
namespace React {

/**
 *  Class definition
 */
class SharedSignal : public Shared<Signal>, public Signal
{
private:
    /**
     *  Called when signal is caught
     */
    virtual void invoke() override
    {
        // keep a shared pointer for as long as the callback is called, this
        // ensures that the object is not destructed if the user calls 
        // the cancel() method, and immediately afterwards a different method
        auto ptr = pointer();
        
        // now we call the base invoke method
        Signal::invoke();
    }

public:
    /**
     *  Constructor
     *  @param  loop
     *  @param  signum
     *  @param  callback
     */
    SharedSignal(MainLoop *loop, int signum, const SignalCallback &callback) : Shared(this), Signal(loop, signum, callback) {}
    
    /**
     *  Destructor
     */
    virtual ~SharedSignal() {}
    
    /**
     *  Start the signal watcher
     *  @return bool
     */
    virtual bool start() override
    {
        // call base
        if (!Signal::start()) return false;
        
        // make sure the shared pointer is valid, so that we have a reference to ourselves
        restore();
        
        // done
        return true;
    }
    
    /**
     *  Cancel the signal watcher
     *  @return bool
     */
    virtual bool cancel() override
    {
        // call base
        if (!Signal::cancel()) return false;
        
        // because the signal watcher is no longer running, we no longer have to keep a pointer to ourselves
        reset();
        
        // done
        return true;
    }
};

/**
 *  End namespace
 */
}
