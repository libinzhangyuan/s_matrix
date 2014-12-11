require "s_matrix/version"

require 's_matrix'


class SMatrix
  def [](index)
    self.get_row(index.to_s)
  end
end
