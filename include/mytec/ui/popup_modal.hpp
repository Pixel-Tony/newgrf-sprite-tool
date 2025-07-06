#ifndef MYTEC_POPUP_MODAL_HPP
#define MYTEC_POPUP_MODAL_HPP

namespace mytec
{
class popup_modal
{
public:
    explicit popup_modal(const char* _title, bool _has_cross, bool _close_on_quit);

    virtual ~popup_modal() = default;

    [[nodiscard]] bool is_open() const noexcept;

    bool update();

protected:
    void close();

    virtual void body() = 0;

private:
    const char* title_;
    bool is_open_ = false;
    bool should_open_ = true;
    const bool has_cross_;

public:
    const bool close_on_quit_;
};
} // namespace mytec

#endif // MYTEC_POPUP_MODAL_HPP
