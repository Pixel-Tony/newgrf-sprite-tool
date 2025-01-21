#ifndef MYTEC_POPUP_MODAL_HPP
#define MYTEC_POPUP_MODAL_HPP


namespace mytec::ui
{
class popup_modal
{
    const char *title;
    bool is_open_ = false;
    bool should_open = false;
    const bool decorate = false;

protected:
    void close();

    virtual void body() = 0;

public:
    explicit popup_modal(const char *title, bool open = false, bool decorate = true);

    virtual ~popup_modal() = default;

    bool is_open() const noexcept;

    void show();

    bool update();
};
}

#endif // MYTEC_POPUP_MODAL_HPP
