class AddQuestionIdToChoixes < ActiveRecord::Migration
  def change
    add_column :choixes, :question_id, :integer
  end
end
